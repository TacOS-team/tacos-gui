#include <screen.h>
#include <window.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <cstdio>
#include <math.h>
#include <string>

using namespace std;

Screen* Screen::instance = NULL;

Screen::Screen(int width, int height, int bitsPerPixel) {
  this->width = width;
  this->height = height;
  this->bitsPerPixel = bitsPerPixel;
  this->bytesPerPixel = bitsPerPixel/8;

  this->vesa_fd = open("/dev/vesa", O_RDONLY);
  struct vesa_setmode_req req = { this->width, this->height, this->bitsPerPixel };
  ioctl(this->vesa_fd, SETMODE, &req);
  ioctl(this->vesa_fd, GETVIDEOADDR, &this->videoBuffer);

  this->clipWin = NULL;
  this->clipZone = new ClipZone(0, 0, this->width, this->height);

  this->gc = &this->defaultGC;

  this->mouseWin = NULL;

  this->tree = new WindowsTree();
}

Screen* Screen::getInstance(int width, int height, int bitsPerPixel) {
  if (Screen::instance == NULL) {
    Screen::instance = new Screen(width, height, bitsPerPixel);
  }
  return instance;
}

Screen* Screen::getInstance() {
  return instance;
}

inline bool Screen::isValid(int x, int y) {
  return (x >= 0 && x < this->width && y >= 0 && y < this->height && this->clipZone->contains(x, y));
}

void Screen::drawPoint(int x, int y, bool check) {
  if (!check || this->isValid(x, y)) {
    memcpy(this->videoBuffer + (y * this->width + x) * 3, &COLOR(this->gc->fg, 24), sizeof(COLOR(this->gc->fg, 24)));
  }
}

void Screen::drawHorizLine(int x, int y, int width, bool check) {
  if (check) {
    check = !this->clipZone->contains(x, y, x + width - 1, y);
  }

  for (int c = 0; c < width; c++) {
    this->drawPoint(x + c, y, check);
  }
}

void Screen::drawVertLine(int x, int y, int height, bool check) {
  if (check) {
    check = !this->clipZone->contains(x, y, x, y + height - 1);
  }

  for (int l = 0; l < height; l++) {
    this->drawPoint(x, y + l, check);
  }
}

void Screen::drawLine(int x1, int y1, int x2, int y2, bool check) {
  if (check) {
    check = !this->clipZone->contains(x1, y1, x2, y2);
  }

  if (x1 == x2) {
    // Vertical line
    this->drawVertLine(x1, y1, (y2 - y1 + 1), check);
  } else if (y1 == y2) {
    // Horizontal line
    this->drawHorizLine(x1, y1, (x2 - x1 + 1), check);
  } else {
    int i, dx, dy, sdx, sdy, dxabs, dyabs, x, y, px, py;

    dx = x2 - x1; 
    dy = y2 - y1;
    dxabs = (dx >= 0 ? dx : -dx);
    dyabs = (dy >= 0 ? dy : -dy);
    sdx = (dx >= 0 ? 1 : -1);
    sdy = (dy >= 0 ? 1 : -1);
    x = dyabs >> 1;
    y = dxabs >> 1;
    px = x1;
    py = y1;

    this->drawPoint(px, py, check);

    if (dxabs >= dyabs) {
      for (i = 0; i < dxabs; i++) {
        y += dyabs;
        if (y >= dxabs) {
          y -= dxabs;
          py += sdy;
        }
        px += sdx;
        this->drawPoint(px, py, check);
      }
    } else { 
      for (i = 0; i < dyabs; i++) {
        x += dxabs;
        if (x >= dyabs) {
          x -= dyabs;
          px += sdx;
        }
        py += sdy;
        this->drawPoint(px, py, check);
      }
    }
  }
}

void Screen::drawRect(int x, int y, int width, int height, bool check) {
  if (check) {
    check = !this->clipZone->contains(x, y, x + width - 1, y + height - 1);
  }

  drawHorizLine(x, y, width, check);
  drawHorizLine(x, y + height - 1, width, check);
  drawVertLine(x, y + 1, height - 2, check);
  drawVertLine(x + width - 1, y + 1, height - 2, check);
}

void Screen::fillRectangle(int x, int y, int width, int height, bool check) {
  if (check) {
    check = !this->clipZone->contains(x, y, x + width - 1, y + height - 1);
  }

  for (int l = 0; l < height; l++) {
    drawHorizLine(x, y + l, width, check);
  }
}

void Screen::putImage(PronImage *image, int x, int y) {
  // We have to test if the image and the screen have the same depth
  //printf("width & height & depth{%d, %d, %d}\n", image->height, image->width, image->depth);
  //printf("absX & absY {%d, %d}\n", srcX + x, srcY + y);
  if(image->depth == this->bitsPerPixel){
    // Copy the image in the video memory
    for (int srcY = 0; srcY < image->height; srcY++) {
      for (int srcX = 0; srcX < image->width; srcX++) {
        if (this->isValid(srcX + x, srcY + y)) {
          // Computing the buffer pointers
          void *src = image->data + (srcX + srcY * image->width) * image->bytesPerPixel;
          void *dest = this->videoBuffer + ( x + y * this->width + srcX + srcY * this->width ) * image->bytesPerPixel;
          //printf("Pointers %p, %p\n", src, dest);
          //printf("Pixel colors {%d, %d, %d}\n", *((char*) src) & ~(0xFFFFFF00), (*((char*) src) >> 8 ) & ~(0xFFFFFF00), (*((char*) src) >> 16) & ~(0xFFFFFF00));
          memcpy(dest, src, image->bytesPerPixel);
        }
      }
    }
  }
}

// source : http://content.gpwiki.org/index.php/SDL:Tutorials:Drawing_and_Filling_Circles
void Screen::drawCircle(int n_cx, int n_cy, int radius) {
  double error = (double) -radius;
  double x = (double) radius - 0.5;
  double y = (double) 0.5;
  double cx = n_cx - 0.5;
  double cy = n_cy - 0.5;

  while (x >= y) {
    this->drawPoint((int)(cx + x), (int)(cy + y));
    this->drawPoint((int)(cx + y), (int)(cy + x));

    if (x != 0) {
      this->drawPoint((int)(cx - x), (int)(cy + y));
      this->drawPoint((int)(cx + y), (int)(cy - x));

      if (y != 0) {
        this->drawPoint((int)(cx + x), (int)(cy - y));
        this->drawPoint((int)(cx - y), (int)(cy + x));
      }

      if (x != 0 && y != 0) {
        this->drawPoint((int)(cx - x), (int)(cy - y));
        this->drawPoint((int)(cx - y), (int)(cy - x));
      }

      error += y;
      ++y;
      error += y;

      if (error >= 0) {
        --x;
        error -= x;
        error -= x;
      }
    }
  }
}

// source : http://content.gpwiki.org/index.php/SDL:Tutorials:Drawing_and_Filling_Circles
void Screen::fillCircle(int cx, int cy, int radius) {
  double r = (double) radius;

  for (double dy = 1; dy <= r; dy += 1.0) {
    double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
    int x = cx - dx;

    for (; x <= cx + dx; x++) {
      this->drawPoint(x, cy + r - dy);
      this->drawPoint(x, cy - r + dy);
    }
  }
}

Drawable* Screen::getDrawable(unsigned int id, int drawableType) {
  for (unsigned int i = 0; i < this->drawables.size(); i++) {
    if (drawables[i]->getId() == id) {
      return (drawables[i]->getType() == drawableType) ? drawables[i] : NULL;
    }
  }
  return NULL;
}

void Screen::addWindow(Window *w) {
  this->drawables.push_back(w);
}

Window* Screen::getMouseWin() {
  return this->mouseWin;
}

void Screen::setMouseWin(Window *mouseWin) {
  this->mouseWin = mouseWin;
}

Window* Screen::getRoot() {
  return this->tree->getRoot();
}

void Screen::setRoot(Window *newRoot) {
  this->tree->setRoot(newRoot);
}

Window* Screen::getClipWin() {
  return this->clipWin;
}

void Screen::printClipZone() {
  this->clipZone->print();
}

void Screen::setClipWin(Window *w) {
  if (w != this->clipWin) {
    // Update clipzone
    if (this->clipZone != NULL) {
      delete this->clipZone;
    }
    if (w == NULL) {
      // Special case : whole screen
      this->clipZone = new ClipZone(0, 0, this->width, this->height);
    } else {
      this->clipZone = new ClipZone(w);
    }
    this->clipWin = w;
  }
}

bool Screen::prepareDrawing(Window *w, GC *gc) {
  if (!w->mapped) {
    // Can't draw in unmapped window
    return false;
  }

  this->setClipWin(w);

  if (gc != NULL) {
    this->gc = gc;
  }

  return true;
}

void traceWindowsRec(Window *w, string prefix) {
  printf("%s%d (p: %d, fc: %d, lc: %d, ps: %d, ns: %d)\n",
        prefix.c_str(),
        w->getId(),
        w->parent == NULL ? 0 : w->parent->getId(),
        w->firstChild == NULL ? 0 : w->firstChild->getId(),
        w->lastChild == NULL ? 0 : w->lastChild->getId(),
        w->prevSibling == NULL ? 0 : w->prevSibling->getId(),
        w->nextSibling == NULL ? 0 : w->nextSibling->getId());
  for (Window *currentChild = w->firstChild; currentChild != NULL; currentChild = currentChild->nextSibling) {
    traceWindowsRec(currentChild, prefix + "--");
  }
}

void Screen::traceWindows() {
  traceWindowsRec(this->tree->getRoot(), "");
}

GC* Screen::getGC(){
  return this->gc;
}

void Screen::setGC(GC *gc){
  this->gc = gc;
}