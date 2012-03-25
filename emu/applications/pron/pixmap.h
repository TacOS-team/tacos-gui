#ifndef _PIXMAP_H_
#define _PIXMAP_H_
#include <pixmap.h>
#include <drawable.h>
#include <screen.h>

#define PIXMAP_BYTES_PER_PIXEL 3
#define PIXMAP_DEPTH 24

class Drawable;
class Screen;

class Pixmap : public Drawable {
public:
  /**
  * Constructors
  */
  Pixmap(Screen *screen, int id, Client *creator, int width, int height);
  ~Pixmap();
private:
  // Image buffer
  char *buf;
};

#endif