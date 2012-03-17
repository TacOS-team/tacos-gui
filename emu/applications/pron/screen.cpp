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

  this->root = NULL;
  this->clipWin = NULL;
  this->clipZone = NULL;

  // Default foreground color
  COLOR(this->gc.fg, 24).r = 255;
  COLOR(this->gc.fg, 24).g = 77;
  COLOR(this->gc.fg, 24).b = 182;

  this->mouseWin = NULL;
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

void Screen::drawPoint(int x, int y) {
  if (x >= 0 && x < this->width && y >= 0 && y < this->height &&  (this->clipZone == NULL || this->clipZone->contains(x, y))) {
    memcpy(this->videoBuffer + (y * this->width + x) * 3, &COLOR(this->gc.fg, 24), sizeof(COLOR(this->gc.fg, 24)));
  }
}

void Screen::drawHorizLine(int x, int y, int width) {
  for (int c = 0; c < width; c++) {
    this->drawPoint(x + c, y);
  }
}

void Screen::drawVertLine(int x, int y, int height) {
  for (int l = 0; l < height; l++) {
    this->drawPoint(x, y + l);
  }
}

void Screen::drawLine(int x1, int y1, int x2, int y2) {
  /*
  float dx, dy, sdx, sdy, dxabs, dyabs, x, y, coordProv, delta;
  int i, px, py;

  color_t c;
  color_t cprov;

  COLOR(c, 24).r = 255;
  COLOR(c, 24).g = 77;
  COLOR(c, 24).b = 182;

  dx = x2 - x1; // the horizontal distance of the line
  dy = y2 - y1; // the vertical distance of the line
  dxabs = (dx >= 0 ? dx : -dx);
  dyabs = (dy >= 0 ? dy : -dy);
  px = x1;
  py = y1;

  // Je l'ai fait que pour ces lignes pour tester
  if (dxabs >= dyabs) { // the line is more horizontal than vertical
    sdx = (dx >= 0 ? 1 : -1);
    sdy = (dy >= 0 ? 1 : -1) * dyabs/dxabs;
    for (i = 0; i < dxabs; i++) {
      printf("dx : %f\n", dx);
      printf("dy : %f\n", dy);
      printf("i : %d\n", i);
      coordProv = y1 + sdy * i;
      printf("coordProv : %f\n", coordProv);
      py = coordProv;
      px += sdx;
      delta = coordProv - py;
      printf("py : %f\n", py);
      printf("delta : %f\n", delta);
      COLOR(cprov, 24).r = delta * COLOR(c, 24).r;
      COLOR(cprov, 24).g = delta * COLOR(c, 24).g;
      COLOR(cprov, 24).b = delta * COLOR(c, 24).b;
      if (delta >= 0.5) {
        this->drawPoint(px, py, cprov);
        printf("avant : %d %d %d\n", COLOR(cprov, 24).r, COLOR(cprov, 24).g, COLOR(cprov, 24).b);
        COLOR(cprov, 24).r = ( 1.0 - delta) * COLOR(c, 24).r;
        COLOR(cprov, 24).g = ( 1.0 - delta) * COLOR(c, 24).g;
        COLOR(cprov, 24).b = ( 1.0 - delta) * COLOR(c, 24).b;
        printf("après : %d %d %d\n", COLOR(cprov, 24).r, COLOR(cprov, 24).g, COLOR(cprov, 24).b);
        this->drawPoint(px, py+1, cprov);
        if (delta >= 0.7) {
          printf("3eme : %d %d %d\n", COLOR(cprov, 24).r, COLOR(cprov, 24).g, COLOR(cprov, 24).b);
          this->drawPoint(px, py-1, cprov);
        }
      } else {
        this->drawPoint(px, py+1, cprov);
        COLOR(cprov, 24).r = ( 1.0 - delta) * COLOR(c, 24).r;
        COLOR(cprov, 24).g = ( 1.0 - delta) * COLOR(c, 24).g;
        COLOR(cprov, 24).b = ( 1.0 - delta) * COLOR(c, 24).b;
        this->drawPoint(px, py, cprov);
        if (delta <= 0.3) {
          this->drawPoint(px, py-1, cprov);
        }
      }
    }
  } else { // the line is more vertical than horizontal
    sdx = (dx >= 0 ? 1 : -1) * dxabs/dyabs;
    sdy = (dy >= 0 ? 1 : -1);
    for (i = 0; i < dyabs; i++) {
      coordProv = x1 + sdx * i;
      px = coordProv;
      py += sdy;
      delta = coordProv - px;
      COLOR(cprov, 24).r = delta * COLOR(c, 24).r;
      COLOR(cprov, 24).g = delta * COLOR(c, 24).g;
      COLOR(cprov, 24).b = delta * COLOR(c, 24).b;
      if (delta >= 0.5) {
        this->drawPoint(px, py, cprov);
        this->drawPoint(px+1, py, c);
      } else {
        this->drawPoint(px, py, c);
        this->drawPoint(px+1, py, cprov);
      }
    }
  }
  // */

  //*
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

  this->drawPoint(px, py);

  if (dxabs >= dyabs) {
    for (i = 0; i < dxabs; i++) {
      y += dyabs;
      if (y >= dxabs) {
        y -= dxabs;
        py += sdy;
      }
      px += sdx;
      this->drawPoint(px, py);
    }
  } else { 
    for (i = 0; i < dyabs; i++) {
      x += dxabs;
      if (x >= dyabs) {
        x -= dyabs;
        px += sdx;
      }
      py += sdy;
      this->drawPoint(px, py);
    }
  }
  // */
}

void Screen::drawRect(int x, int y, int width, int height) {
  drawHorizLine(x, y, width);
  drawHorizLine(x, y + height - 1, width);
  drawVertLine(x, y + 1, height - 2);
  drawVertLine(x + width - 1, y + 1, height - 2);
}

void Screen::fillRectangle(int x, int y, int width, int height) {
  for (int l = 0; l < height; l++) {
    drawHorizLine(x, y + l, width);
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

Window* Screen::getWindow(unsigned int id) {
  for (unsigned int i = 0; i < this->windows.size(); i++) {
    if (windows[i]->id == id) {
      return windows[i];
    }
  }
  return NULL;
}

void Screen::addWindow(Window *w) {
  this->windows.push_back(w);
}

Window* Screen::getMouseWin() {
  return this->mouseWin;
}

void Screen::setMouseWin(Window *mouseWin) {
  this->mouseWin = mouseWin;
}

Window* Screen::getRoot() {
  return this->root;
}

void Screen::setRoot(Window *root) {
  this->root = root;
}

void Screen::setClipWindow(Window *w) {
  if (w != this->clipWin) {
    // Update clipzone
    if (this->clipZone != NULL) {
      delete this->clipZone;
    }
    this->clipZone = new ClipZone(w);
    this->clipWin = w;
  }
}

bool Screen::prepareDrawing(Window *w) {
  if (!w->mapped) {
    // Can't draw in unmapped window
    return false;
  }

  this->setClipWindow(w);

  return true;
}

void traceWindowsRec(Window *w, string prefix) {
  printf("%s%d (p: %d, fc: %d, lc: %d, ps: %d, ns: %d)\n",
        prefix.c_str(),
        w->id,
        w->parent == NULL ? 0 : w->parent->id,
        w->firstChild == NULL ? 0 : w->firstChild->id,
        w->lastChild == NULL ? 0 : w->lastChild->id,
        w->prevSibling == NULL ? 0 : w->prevSibling->id,
        w->nextSibling == NULL ? 0 : w->nextSibling->id);
  for (Window *currentChild = w->firstChild; currentChild != NULL; currentChild = currentChild->nextSibling) {
    traceWindowsRec(currentChild, prefix + "--");
  }
}

void Screen::traceWindows() {
  traceWindowsRec(this->root, "");
}
