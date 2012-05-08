/**
 * @file screen.cpp
 * Screen class implementation.
 */

#include <algorithm>
#include <dirent.h>
#include <fcntl.h>
//#include <math.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <sys/ioctl.h>

#include <font_loader.h>
#include <screen.h>
#include <window.h>

using namespace std;

Screen* Screen::instance = NULL;

Screen::Screen(int width, int height, int bitsPerPixel) {
  this->width = width;
  this->height = height;
  this->bitsPerPixel = bitsPerPixel;
  this->bytesPerPixel = bitsPerPixel/8;

  this->vesa_fd = open("/dev/vesa", O_RDONLY);
  struct vesa_setmode_req req = { this->width, this->height, this->bitsPerPixel };
  ioctl(this->vesa_fd, SETMODE, &req);
  ioctl(this->vesa_fd, GETVIDEOADDR, &this->videoBuffer);

  DIR *dp;
  struct dirent *ep;  
  dp = opendir("fonts");
  if (dp != NULL) {
    while ((ep = readdir(dp))) {
      if (ep->d_type == DT_REG) {
        char fileName[256] = "fonts/";
        strcat(fileName, ep->d_name);
        this->fonts.push_back(FontLoader::load(fileName));
      }
    }
    closedir (dp);
  } else {
    fprintf(stderr, "Could not open the fonts directory.\n");
    exit(1);
  }

  this->clipWin = NULL;
  this->clipZone = new ClipZone(0, 0, this->width, this->height);
  this->clippingCheck = true;

  this->gc = &this->defaultGC;

  this->mouseWin = NULL;
  this->focusWin = NULL;

  this->tree = new WindowsTree();
}

Screen* Screen::getInstance(int width, int height, int bitsPerPixel) {
  if (Screen::instance == NULL) {
    Screen::instance = new Screen(width, height, bitsPerPixel);
  }
  return instance;
}

Screen* Screen::getInstance() {
  return instance;
}

void Screen::setClippingCheck(bool clippingCheck) {
  this->clippingCheck = clippingCheck;
}

bool Screen::isValid(int x, int y) {
  return (x >= 0 && x < this->width && y >= 0 && y < this->height &&
      (!this->clippingCheck || this->clipZone->contains(x, y)));
}

void* Screen::pixelAddr(int x, int y) {
  return this->videoBuffer + (y * this->width + x) * 3;
}

Drawable* Screen::getDrawable(unsigned int id, int drawableType) {
  for (unsigned int i = 0; i < this->drawables.size(); i++) {
    if (drawables[i]->getId() == id) {
      return (drawables[i]->getType() == drawableType) ? drawables[i] : NULL;
    }
  }
  return NULL;
}

Drawable* Screen::getDrawable(unsigned int id) {
  for (unsigned int i = 0; i < this->drawables.size(); i++) {
    if (drawables[i]->getId() == id) {
      return drawables[i];
    }
  }
  return NULL;
}

void Screen::addDrawable(Drawable *d) {
  this->drawables.push_back(d);
}

void Screen::removeDrawable(Drawable *d) {
  this->drawables.erase(std::find(this->drawables.begin(), this->drawables.end(), d));
}

Window* Screen::getMouseWin() {
  return this->mouseWin;
}

void Screen::setMouseWin(Window *mouseWin) {
  this->mouseWin = mouseWin;
}

Window* Screen::getFocusWin() {
  return this->focusWin;
}

void Screen::setFocusWin(Window *focusWin) {
  this->focusWin = focusWin;
}

Window* Screen::getRoot() {
  return this->tree->getRoot();
}

Window* Screen::getClipWin() {
  return this->clipWin;
}

ClipZone* Screen::getClipZone() {
  return this->clipZone;
}

void Screen::printClipZone() {
  this->clipZone->print();
}

void Screen::setClipWin(Window *w) {
  if (w != this->clipWin) {
    // Update clipzone
    if (this->clipZone != NULL) {
      delete this->clipZone;
    }
    if (w == NULL) {
      // Special case : whole screen
      this->clipZone = new ClipZone(0, 0, this->width, this->height);
    } else {
      this->clipZone = new ClipZone(w);
    }
    this->clipWin = w;
  }
}

bool Screen::prepareDrawing(Drawable *d, GC *gc) {
  if (d->getType() == D_WINDOW) {
    Window *w = (Window*) d;

    if (!w->realized()) {
      // Can't draw in unrealized window
      return false;
    }

    this->setClipWin(w);
    this->setClippingCheck(true);
  }

  if (gc != NULL) {
    this->gc = gc;
  }

  return true;
}

/**
 * Utility function used by Screen::traceWindows.
 * @param w The current window to show
 * @param prefix The current prefix to show
 */
void traceWindowsRec(Window *w, string prefix) {
  printf("%s%x (p: %x, fc: %x, lc: %x, ps: %x, ns: %x, r: %s)\n",
        prefix.c_str(),
        w->getId(),
        w->parent == NULL ? 0 : w->parent->getId(),
        w->firstChild == NULL ? 0 : w->firstChild->getId(),
        w->lastChild == NULL ? 0 : w->lastChild->getId(),
        w->prevSibling == NULL ? 0 : w->prevSibling->getId(),
        w->nextSibling == NULL ? 0 : w->nextSibling->getId(),
        w->realized() ? "yes" : "no");
  for (Window *currentChild = w->firstChild; currentChild != NULL; currentChild = currentChild->nextSibling) {
    traceWindowsRec(currentChild, prefix + "--");
  }
}

void Screen::traceWindows() {
  traceWindowsRec(this->tree->getRoot(), "");
  /*printf("TraceWindow : ");
  for (WindowsTree::IteratorDFS it = tree->beginDFS() ; it != tree->endDFS(); it++) {
    printf("%d ", it->getId());
  }
  */
  printf("\n");
}

GC* Screen::getGC(){
  return this->gc;
}

void Screen::setGC(GC *gc){
  this->gc = gc;
}

void Screen::destroy(Window * w) {
  for (WindowsTree::IteratorBFS it = this->tree->beginBFS(w); it != this->tree->endBFS(); it++) {
    EventDestroyWindow eventDestroyWindow(it->getId());
    it->deliverWindowEvent(&eventDestroyWindow, sizeof(eventDestroyWindow));
  }
}

void Screen::reparent (Window * child, Window * newParent) {
  child->reparent(newParent);
  this->setClipWin(NULL);
  this->traceWindows();
}

Font* Screen::getFont(int id) {
  return this->fonts[id];
}
