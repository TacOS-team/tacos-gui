#include <pixmap.h>
#include <unistd.h>
#include <pronlib_enums.h>

Pixmap::Pixmap(Screen *screen, int id, Client *creator, int width, int height) 
    : Drawable(D_PIXMAP, screen, id, creator, width, height) {
  this->buf = (char*) malloc(PIXMAP_BYTES_PER_PIXEL * width * height);
}

Pixmap::~Pixmap() {
  free(this->buf);
}