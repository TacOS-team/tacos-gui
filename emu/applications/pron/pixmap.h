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
      memcpy(this->buf + (Y) * this->getWidth() + (X) * PIXMAP_BYTES_PER_PIXEL, \
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
  Pixmap(Screen *screen, int id, Client *creator, int width, int height);
  ~Pixmap();

  /**
   * Graphic methods
   */
  void drawPoint(int x, int y);
  void drawLine(int x1, int y1, int x2, int y2);
  void drawRect(int x1, int y1, int width, int height);
  void fillRectangle(int x, int y, int width, int height);
  void putImage(PronImage *image, int x, int y);
  void drawCircle(int x, int y, int radius);
  void fillCircle(int x, int y, int radius);
  void clear();
  void clear(int x, int y, int width, int height);

private:
  // Image buffer
  char *buf;

  void drawHorizLine(int x, int y, int width);
  void drawVertLine(int x, int y, int height);
};

#endif