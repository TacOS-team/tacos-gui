#ifndef _PIXMAP_H_
#define _PIXMAP_H_
#include <pixmap.h>
#include <drawable.h>
#include <screen.h>

#define PIXMAP_BYTES_PER_PIXEL 3
#define PIXMAP_DEPTH 24

/**
 * Macro to draw a pixel using the screen
 * and the graphic context
 */
#define PIXMAP_DRAW_POINT(X, Y) \
    if ((X) < this->getWidth() && (Y) < this->getHeight() && \
        (X) >= 0 && (Y) >= 0) { \
      memcpy(this->buf + ((Y) * this->getWidth() + (X) )* PIXMAP_BYTES_PER_PIXEL, \
          &COLOR(this->getScreen()->getGC()->fg, PIXMAP_DEPTH), \
          sizeof(COLOR(this->getScreen()->getGC()->fg, PIXMAP_DEPTH))); \
    }

class Drawable;
class Screen;

class Pixmap : public Drawable {
public:
  /**
  * Constructors
  */
  Pixmap(Screen *screen, int id, Client *creator, int width, int height, int depth);
  ~Pixmap();

  /**
   * Graphic methods
   */
  void* pixelAddr(int x, int y);

  void putImage(PronImage *image, int x, int y);
  
  int getPixel(int x, int y);
  void setPixel(int x, int y, int pixel);

  bool isValid(int x, int y);

private:
  // Image buffer
  char *buf;
  // Deth of the pixmap
  int depth;
};

#endif
