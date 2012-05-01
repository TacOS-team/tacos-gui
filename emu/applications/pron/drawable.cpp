/**
 * @file drawable.cpp
 * Drawable class implementation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <client.h>
#include <drawable.h>
#include <screen.h>
#include <window.h>

Drawable::Drawable(int type, Screen *screen, int id, Client *creator, int width, int height) {
  this->type = type;
  this->screen = screen;
  this->id = id;
  this->creator = creator;
  this->width = width;
  this->height = height;

  // Add the new drawable to the screen's list of drawables
  screen->addDrawable(this);
}

Drawable::~Drawable() {
  // Destruction
  this->getScreen()->removeDrawable(this);
}

int Drawable::getType() {
  return this->type;
}

Screen* Drawable::getScreen() {
  return this->screen;
}

unsigned int Drawable::getId() const {
  return this->id;
}

Client* Drawable::getCreator() {
  return this->creator;
}

int Drawable::getWidth() {
  return this->width;
}

void Drawable::setWidth(int width) {
  this->width = width;
}

int Drawable::getHeight() {
  return this->height;
}

void Drawable::setHeight(int height) {
  this->height = height;
}

void Drawable::reduce(int &x, int &y, int &width, int &height) {
  if (x < 0) {
    width += x;
    x = 0;
  }

  if (y < 0) {
    height += y;
    y = 0;
  }

  if (x + width > this->getWidth()) {
    width = this->getWidth() - x;
  }

  if (y + height > this->getHeight()) {
    height = this->getHeight() - y;
  }
}

void Drawable::drawPoint(int x, int y) {
  if (this->isValid(x, y)) {
    memcpy(this->pixelAddr(x, y), &COLOR(this->getScreen()->getGC()->fg, 24), sizeof(COLOR(this->getScreen()->getGC()->fg, 24)));
  }
}

void Drawable::drawHorizLine(int x, int y, int width) {
  for (int c = 0; c < width; c++) {
    this->drawPoint(x + c, y);
  }
}

void Drawable::drawVertLine(int x, int y, int height) {
  for (int l = 0; l < height; l++) {
    this->drawPoint(x, y + l);
  }
}

void Drawable::drawLine(int x1, int y1, int x2, int y2) {
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
  }
}

void Drawable::drawRect(int x, int y, int width, int height){
  drawHorizLine(x, y, width);
  drawHorizLine(x, y + height - 1, width);
  drawVertLine(x, y + 1, height - 2);
  drawVertLine(x + width - 1, y + 1, height - 2);
}

void Drawable::fillRectangle(int x, int y, int width, int height){
  for (int l = 0; l < height; l++) {
    drawHorizLine(x, y + l, width);
  }
}

void Drawable::drawCircle(int n_cx, int n_cy, int radius){
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

void Drawable::fillCircle(int cx __attribute__((unused)), int cy __attribute__((unused)), int radius __attribute__((unused))){
  /*double r = (double) radius;

  for (double dy = 1; dy <= r; dy += 1.0) {
    double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
    int x = cx - dx;

    for (; x <= cx + dx; x++) {
      this->drawPoint(x, (int)(cy + r - dy));
      this->drawPoint(x, (int)(cy - r + dy));
    }
  }*/
}

int Drawable::getPixel(int x, int y) {
  if (this->isValid(x, y)) {
    int ret;
    memcpy(&ret, this->pixelAddr(x, y), this->getScreen()->bytesPerPixel);
    return ret;
  }
  return -1;
}

void Drawable::setPixel(int x, int y, int pixel) {
  if (this->isValid(x, y)) {
    memcpy(this->pixelAddr(x, y), &pixel, this->getScreen()->bytesPerPixel);
  }
}

void Drawable::putImage(PronImage *image, int x, int y) {
  // We have to test if the image and the drawable have the same depth
  if (image->depth == this->getScreen()->bitsPerPixel) {
    // Copy the image in the video memory
    for (int srcY = 0; srcY < image->height; srcY++) {
      for (int srcX = 0; srcX < image->width; srcX++) {
        if (this->isValid(srcX + x, srcY + y)) {
          // Computing the buffer pointers
          void *src = image->data + (srcX + srcY * image->width) * image->bytesPerPixel;
          void *dest = this->pixelAddr(x + srcX, y + srcY);
          memcpy(dest, src, image->bytesPerPixel);
        }
      }
    }
  }
}

void Drawable::copyArea(int dstX, int dstY, Drawable *d, int srcX, int srcY, int width, int height) {
  /** @todo XXX Bourrin à revoir (problème de depth et de byte per pixel de la pixmap et de l'écran) */
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      this->setPixel(x + dstX, y + dstY, d->getPixel(x + srcX, y + srcY));
    }
  }
}

void Drawable::drawText(int x, int y, const char *text, int length) {
  Font *font = this->getScreen()->getFont(this->getScreen()->getGC()->font_num);
  font->drawText(this, x, y, text, length);
}
