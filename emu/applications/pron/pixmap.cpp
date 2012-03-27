#include <pixmap.h>
#include <math.h>
#include <cstdio>
#include <string.h>
#include <algorithm>
#include <drawable.h>

using namespace std;

Pixmap::Pixmap(Screen *screen, int id, Client *creator, int width, int height, int depth) 
    : Drawable(D_PIXMAP, screen, id, creator, width, height) {
  this->buf = (char*) malloc(PIXMAP_BYTES_PER_PIXEL * width * height);
  this->depth = depth;
}

Pixmap::~Pixmap() {
  this->getScreen()->drawables.erase(std::find(this->getScreen()->drawables.begin(), this->getScreen()->drawables.end(), (Drawable*) this));
  free(this->buf);
}

void Pixmap::drawPoint(int x, int y) {
  PIXMAP_DRAW_POINT(x, y);// Cf pixmap.h
}

void Pixmap::drawHorizLine(int x, int y, int width) {
  for (int c = 0; c < width; c++) {
    PIXMAP_DRAW_POINT(x + c, y);
  }
}

void Pixmap::drawVertLine(int x, int y, int height) {
  for (int l = 0; l < height; l++) {
    PIXMAP_DRAW_POINT(x, y + l);
  }
}

void Pixmap::drawLine(int x1, int y1, int x2, int y2) {
  if (x1 == x2) {
  // Vertical line
    this->drawVertLine(x1, y1, (y2 - y1 + 1));
  } else if (y1 == y2) {
    // Horizontal line
    this->drawHorizLine(x1, y1, (x2 - x1 + 1));
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

    PIXMAP_DRAW_POINT(px, py);

    if (dxabs >= dyabs) {
      for (i = 0; i < dxabs; i++) {
        y += dyabs;
        if (y >= dxabs) {
          y -= dxabs;
          py += sdy;
        }
        px += sdx;
        PIXMAP_DRAW_POINT(px, py);
      }
    } else { 
      for (i = 0; i < dyabs; i++) {
        x += dxabs;
        if (x >= dyabs) {
          x -= dyabs;
          px += sdx;
        }
        py += sdy;
        PIXMAP_DRAW_POINT(px, py);
      }
    }
  }
}

void Pixmap::drawRect(int x, int y, int width, int height){
  drawHorizLine(x, y, width);
  drawHorizLine(x, y + height - 1, width);
  drawVertLine(x, y + 1, height - 2);
  drawVertLine(x + width - 1, y + 1, height - 2);
}

void Pixmap::fillRectangle(int x, int y, int width, int height){
  for (int l = 0; l < height; l++) {
    drawHorizLine(x, y + l, width);
  }
}

void Pixmap::putImage(PronImage *image, int x, int y){
  // We have to test if the image and the pixmap have the same depth
  //printf("width & height & depth{%d, %d, %d}\n", image->height, image->width, image->depth);
  //printf("absX & absY {%d, %d}\n", srcX + x, srcY + y);
  if(image->depth == PIXMAP_DEPTH){
    // Copy the image in the video memory
    for (int srcY = 0; srcY < image->height; srcY++) {
      for (int srcX = 0; srcX < image->width; srcX++) {
        if (srcX + x < this->getWidth() && srcY + y < this->getHeight() &&
            x >= 0 && y >= 0) {
          // Computing the buffer pointers
          void *src = image->data + (srcX + srcY * image->width) * image->bytesPerPixel;
          void *dest = this->buf + ( x + y * this->getWidth() + srcX + srcY * this->getWidth() ) * image->bytesPerPixel;
          //printf("Pointers %p, %p\n", src, dest);
          //printf("Pixel colors {%d, %d, %d}\n", *((char*) src) & ~(0xFFFFFF00), (*((char*) src) >> 8 ) & ~(0xFFFFFF00), (*((char*) src) >> 16) & ~(0xFFFFFF00));
          memcpy(dest, src, image->bytesPerPixel);
        }
      }
    }
  }
}

void Pixmap::drawCircle(int n_cx, int n_cy, int radius){
  double error = (double) -radius;
  double x = (double) radius - 0.5;
  double y = (double) 0.5;
  double cx = n_cx - 0.5;
  double cy = n_cy - 0.5;

  while (x >= y) {
    PIXMAP_DRAW_POINT((int)(cx + x), (int)(cy + y));
    this->drawPoint((int)(cx + y), (int)(cy + x));

    if (x != 0) {
      PIXMAP_DRAW_POINT((int)(cx - x), (int)(cy + y));
      PIXMAP_DRAW_POINT((int)(cx + y), (int)(cy - x));

      if (y != 0) {
        PIXMAP_DRAW_POINT((int)(cx + x), (int)(cy - y));
        PIXMAP_DRAW_POINT((int)(cx - y), (int)(cy + x));
      }

      if (x != 0 && y != 0) {
        PIXMAP_DRAW_POINT((int)(cx - x), (int)(cy - y));
        PIXMAP_DRAW_POINT((int)(cx - y), (int)(cy - x));
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

void Pixmap::fillCircle(int cx, int cy, int radius){
  double r = (double) radius;

  for (double dy = 1; dy <= r; dy += 1.0) {
    double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
    int x = cx - dx;

    for (; x <= cx + dx; x++) {
      PIXMAP_DRAW_POINT(x, (int)(cy + r - dy));
      PIXMAP_DRAW_POINT(x, (int)(cy - r + dy));
    }
  }
}

void Pixmap::clear(int x, int y, int width, int height){
  this->reduce(x, y, width, height);

  GC *gc = this->getScreen()->getGC();
  color_t oldFg = gc->fg;
  COLOR(gc->fg, 24).r = 255;
  COLOR(gc->fg, 24).g = 255;
  COLOR(gc->fg, 24).b = 255;
  this->fillRectangle(x, y, width, height);
  gc->fg = oldFg;
}

void Pixmap::clear(){
  this->clear(0, 0, this->getWidth(), this->getHeight());
}
