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

void Pixmap::putImage(PronImage *image, int x, int y){
  // We have to test if the image and the pixmap have the same depth
  if(image->depth == PIXMAP_DEPTH){
    // Copy the image in the video memory
    for (int srcY = 0; srcY < image->height; srcY++) {
      for (int srcX = 0; srcX < image->width; srcX++) {
        if (srcX + x < this->getWidth() && srcY + y < this->getHeight() &&
            x >= 0 && y >= 0) {
          // Computing the buffer pointers
          void *src = image->data + (srcX + srcY * image->width) * image->bytesPerPixel;
          void *dest = this->buf + ( x + y * this->getWidth() + srcX + srcY * this->getWidth() ) * image->bytesPerPixel;
          memcpy(dest, src, image->bytesPerPixel);
        }
      }
    }
  }
}


int Pixmap::getPixel(int x, int y) {
  if (x < this->getWidth() && y < this->getHeight() && x >= 0 && y >= 0) {
    int ret = 0;
    memcpy(&ret, this->buf + (y * this->getWidth() + x) * PIXMAP_BYTES_PER_PIXEL, PIXMAP_BYTES_PER_PIXEL);
    return ret; 
  }
  return -1;
}

void Pixmap::setPixel(int x, int y, int pixel) {
  if (x < this->getWidth() && y < this->getHeight() && x >= 0 && y >= 0) {
    memcpy(this->buf + (y * this->getWidth() + x) * PIXMAP_BYTES_PER_PIXEL, &pixel, PIXMAP_BYTES_PER_PIXEL);
  }
}
