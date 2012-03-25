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

void Pixmap::drawPoint(int x, int y){/*TODO unimplemented*/}
void Pixmap::drawLine(int x1, int y1, int x2, int y2){/*TODO unimplemented*/}
void Pixmap::drawRect(int x1, int y1, int width, int height){/*TODO unimplemented*/}
void Pixmap::fillRectangle(int x, int y, int width, int height){/*TODO unimplemented*/}
void Pixmap::putImage(PronImage *image, int x, int y){/*TODO unimplemented*/}
void Pixmap::drawCircle(int x, int y, int radius){/*TODO unimplemented*/}
void Pixmap::fillCircle(int x, int y, int radius){/*TODO unimplemented*/}
void Pixmap::clear(){/*TODO unimplemented*/}
void Pixmap::clear(int x, int y, int width, int height){/*TODO unimplemented*/}