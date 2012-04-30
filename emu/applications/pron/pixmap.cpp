#include <algorithm>
//#include <math.h>
#include <stdio.h>
#include <string.h>

#include <drawable.h>
#include <pixmap.h>

using namespace std;

Pixmap::Pixmap(Screen *screen, int id, Client *creator, int width, int height, int depth) 
    : Drawable(D_PIXMAP, screen, id, creator, width, height) {
  this->buf = (char*) malloc(PIXMAP_BYTES_PER_PIXEL * width * height);
  this->depth = depth;
}

Pixmap::~Pixmap() {
  free(this->buf);
}

void* Pixmap::pixelAddr(int x, int y) {
  return this->buf + (y * this->getWidth() + x) * PIXMAP_BYTES_PER_PIXEL;
}

bool Pixmap::isValid(int x, int y) {
  return (x >= 0 && x < this->getWidth() && y >= 0 && y < this->getHeight());
}
