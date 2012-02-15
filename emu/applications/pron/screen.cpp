#include <screen.h>
#include <window.h>
#include <sys/ioctl.h>
#include <fcntl.h>
extern "C" {
#include <vesa_types.h>
}
#include <string.h>
#include <cstdio>

#define _COLOR(_p, _bpp) _p.bpp ## _bpp
#define COLOR(_p, _bpp) _COLOR(_p, _bpp)

Screen::Screen(int width, int height, int bitsPerPixel) {
  this->width = width;
  this->height = height;
  this->bitsPerPixel = bitsPerPixel;
  this->bytesPerPixel = bitsPerPixel/8;

  this->vesa_fd = open("/dev/vesa", O_RDONLY);
  struct vesa_setmode_req req = { this->width, this->height, this->bitsPerPixel };
  ioctl(this->vesa_fd, SETMODE, &req);
  ioctl(this->vesa_fd, GETVIDEOADDR, &this->videoBuffer);
}

// TODO: couleurs
void Screen::drawPoint(int x, int y/*, color_t c*/) {
  // XXX: constantes en dur moches
  color_t c;
  COLOR(c, 24).r = 255;
  COLOR(c, 24).g = 77;
  COLOR(c, 24).b = 182;
  if (x >= 0 && x < this->width && y >= 0 && y < this->height) {
    memcpy(this->videoBuffer + (y * this->width + x) * 3, &COLOR(c, 24), sizeof(COLOR(c, 24)));
  }
}

// TODO: couleurs
void Screen::drawLine(int x1, int y1, int x2, int y2/*, color_t color*/) {
  int i, dx, dy, sdx, sdy, dxabs, dyabs, x, y, px, py;

  dx = x2 - x1; /* the horizontal distance of the line */
  dy = y2 - y1; /* the vertical distance of the line */
  dxabs = (dx >= 0 ? dx : -dx);
  dyabs = (dy >= 0 ? dy : -dy);
  sdx = (dx >= 0 ? 1 : -1);
  sdy = (dy >= 0 ? 1 : -1);
  x = dyabs >> 1;
  y = dxabs >> 1;
  px = x1;
  py = y1;

  if (dxabs >= dyabs) { /* the line is more horizontal than vertical */
    for (i = 0; i < dxabs; i++) {
      y += dyabs;
      if (y >= dxabs) {
	y -= dxabs;
	py += sdy;
      }
      px += sdx;
      this->drawPoint(px, py/*, color*/);
    }
  } else { /* the line is more vertical than horizontal */
    for (i = 0; i < dyabs; i++) {
      x += dxabs;
      if (x >= dyabs) {
	x -= dyabs;
	px += sdx;
      }
      py += sdy;
      this->drawPoint(px, py/*, color*/);
    }
  }
}

// TODO: couleurs
void Screen::drawRect(int x, int y, int width, int height/*, color_t color*/) {
  //test the points
  if( x >= 0 && y >= 0 &&
      x < this->width && x + width < this->width &&
      y < this->height && y + height < this->height ) {
    // XXX: the pixel which is drawn
    color_t c;
    COLOR(c, 24).r = 255;
    COLOR(c, 24).g = 77;
    COLOR(c, 24).b = 182;

    int pix = 0;
    for (pix = x; pix <= x + width; pix++) {
      this->drawPoint(pix, y/*, color */);
      this->drawPoint(pix, y + height/*, color */);
    }
    for (pix = y; pix <= y + height; pix++) {
      this->drawPoint(x, pix/*, color */);
      this->drawPoint(x + width, pix/*, color */);
    }

  }
}


// TODO: couleurs
void Screen::fillRectangle(int x, int y, int width, int height/*, color_t color*/) {
  //test the points
  if( x >= 0 && y >= 0 &&
      x < this->width && x + width < this->width &&
      y < this->height && y + height < this->height ) {
    // XXX: the pixel which is drawn
    color_t c;
    COLOR(c, 24).r = 255;
    COLOR(c, 24).g = 77;
    COLOR(c, 24).b = 182;
    //current line
    int l;
    //current pixel
    int p;
    for(l = 0 ; l < height ; l++){
      //draw the line 
      for(p = 0 ; p < width ; p++){
	//draw a pixel
	memcpy(this->videoBuffer + ((l + y) * this->width + p + x) * sizeof(COLOR(c, 24)) , &COLOR(c,24), sizeof(COLOR(c, 24)));
      }
    }
  }
}

Window* Screen::getWindow(int id) {
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

void Screen::flush() {
  ioctl(this->vesa_fd, FLUSH, 0);
}
