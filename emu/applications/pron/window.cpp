#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <client.h>
#include <drawable.h>
#include <mouse.h>
#include <screen.h>
#include <window.h>

Window::Window(Screen *screen, int id, Client *creator, Window *parent, int x, int y, int width, int height) 
    : Drawable(D_WINDOW, screen, id, creator, width, height) {
  this->x = x;
  this->y = y;
  memset(&this->bgColor, 0, sizeof(this->bgColor));
  COLOR(this->bgColor, 24).r = 0;
  COLOR(this->bgColor, 24).g = 0;
  COLOR(this->bgColor, 24).b = 0;
  this->eventMask = 0;
  this->dontPropagateMask = 0;
  this->mapped = false;
  this->isResizable = true;
  this->maxWidth = -1; // -1 means there is no limit
  this->maxHeight = -1;
  this->minWidth = -1;
  this->minHeight = -1;

  // Père
  this->parent = parent;
  // Fils
  this->firstChild = this->lastChild = NULL;
  // Frères
  if (this->parent != NULL) {
    this->prevSibling = parent->lastChild;
    this->nextSibling = NULL;
  } else {
    this->prevSibling = this->nextSibling = NULL;
  }
  // Maj du père
  if (this->parent != NULL) {
    if (this->parent->lastChild == NULL) {
      this->parent->firstChild = this->parent->lastChild = this;
    } else {
      this->parent->lastChild->nextSibling = this;
      this->parent->lastChild = this;
    }
  }
  
  if (this->parent != NULL) {
    // Send an event notifiying the window creation
    EventWindowCreated eventCreated(this->getId(), this->parent->getId(), this->getAttributes());
    this->deliverWindowEvent(&eventCreated, sizeof(eventCreated));
  }
}

// Destructor : delete all of the childs
Window::~Window() {
  for (Window *child = this->firstChild, *nextChild; child != NULL; child = nextChild) {
    nextChild = child->nextSibling;
    delete child;
  }
}

bool Window::operator==(const Window &w) const {
  return this->getId() == w.getId();
}

bool Window::operator!=(const Window &w) const {
  return !(*this == w);
}

void Window::unmap() {
  // Can't unmap root window
  if (this->parent == NULL) {
    return;
  }

  this->mapped = false;

  // Unmap all children
  for (Window *child = this->firstChild; child != NULL; child = child->nextSibling) {
    child->unmap();
  }

  // TODO: check if we were clipwin/mousewin/focuswin
  if (this->getScreen()->getClipWin() == this) {
    this->getScreen()->setClipWin(NULL);
  }

  if (this->getScreen()->getMouseWin() == this) {
    Mouse::getInstance()->updateMouseWin();
  }

  if (this->parent->mapped) {
    // Clear the area of the parent window occupied by this window and send exposure event
    this->parent->clear(this->x, this->y, this->getWidth(), this->getHeight());

    // Redraw covered lower siblings
    for (Window *sib = this->prevSibling; sib != NULL; sib = sib->prevSibling) {
      if (this->overlaps(sib)) {
        sib->clear(this->x - sib->x, this->y - sib->y, this->getWidth(), this->getHeight());
      }
    }
  }
}

void Window::map() {
  this->mapped = true;

  // TODO: update clipwin/mousewin/focuswin

  // Clear the window and send exposure event
  //this->clear();
  EventExpose expose(this->getId(), 0, 0, this->getWidth(), this->getHeight());
  this->deliverEvent(&expose, sizeof(expose));

  // Map all children
  for (Window *child = this->firstChild; child != NULL; child = child->nextSibling) {
    child->map();
  }
}

void Window::drawPoint(int x, int y) {
  this->getScreen()->drawPoint(this->x + x, this->y + y);
}

void Window::drawLine(int x1, int y1, int x2, int y2) {
  this->getScreen()->drawLine(this->x + x1, this->y + y1, this->x + x2, this->y + y2);
}

void Window::drawRect(int x1, int y1, int width, int height) {
  this->getScreen()->drawRect(this->x + x1, this->y + y1, width, height);
}

void Window::fillRectangle(int x1, int y1, int width, int height) {
  this->getScreen()->fillRectangle(this->x + x1, this->y + y1, width, height);
}

void Window::putImage(PronImage *image, int x, int y) {
  this->getScreen()->putImage(image, this->x + x, this->y + y);
}

void Window::drawCircle(int x, int y, int radius) {
  this->getScreen()->drawCircle(this->x + x, this->y + y, radius); 
}

void Window::fillCircle(int x, int y, int radius) {
  this->getScreen()->fillCircle(this->x + x, this->y + y, radius); 
}

void Window::clear(int x, int y, int width, int height) {
  this->reduce(x, y, width, height);

  color_t oldFg = this->getScreen()->gc->fg;
  COLOR(this->getScreen()->gc->fg, 24).r = COLOR(this->bgColor, 24).r;
  COLOR(this->getScreen()->gc->fg, 24).g = COLOR(this->bgColor, 24).g;
  COLOR(this->getScreen()->gc->fg, 24).b = COLOR(this->bgColor, 24).b;
  this->getScreen()->setClipWin(this);
  this->getScreen()->fillRectangle(this->x + x, this->y + y, width, height);
  // If it is the root window, we print a grid (provisoire !!!!!! TODO)
  if (this->getId() == 0) {
    COLOR(this->getScreen()->gc->fg, 24).r = 255;
    COLOR(this->getScreen()->gc->fg, 24).g = 0;
    COLOR(this->getScreen()->gc->fg, 24).b = 0;
    int step = 50;
    for (int i = step; i < this->getWidth(); i += step) {
      this->getScreen()->drawLine(i, 0, i, this->getHeight());
    }
    for (int i = step; i < this->getHeight(); i += step) {
      this->getScreen()->drawLine(0, i, this->getWidth(), i);
    }
  }
  this->getScreen()->gc->fg = oldFg;

  // Send exposure event
  EventExpose expose(this->getId(), x, y, width, height);
  this->deliverEvent(&expose, sizeof(expose));
}

void Window::clear() {
  this->clear(0, 0, this->getWidth(), this->getHeight());
}

PronWindowAttributes Window::getAttributes() {
  PronWindowAttributes attr;
  attr.x = this->x;
  attr.y = this->y;
  attr.width = this->getWidth();
  attr.height = this->getHeight();
  attr.bgColor = this->bgColor;
  attr.isResizable = this->isResizable;
  attr.maxWidth = this->maxWidth;
  attr.maxHeight = this->maxHeight;
  attr.minWidth = this->minWidth;
  attr.minHeight = this->minHeight;

  return attr;
}

int Window::getPixel(int x, int y) {
  return this->getScreen()->getPixel(this->x + x, this->y + y);
}

void Window::setPixel(int x, int y, int pixel) {
  this->getScreen()->setPixel(this->x + x, this->y + y, pixel);
}

void Window::setAttributes(PronWindowAttributes *newAttr, unsigned int mask) {
  if (mask & WIN_ATTR_X) {
    this->x = newAttr->x;
  }
  if (mask & WIN_ATTR_Y) {
    this->y = newAttr->y;
  }
  if (mask & WIN_ATTR_WIDTH) {
    this->setWidth(newAttr->width);
  }
  if (mask & WIN_ATTR_HEIGHT) {
    this->setHeight(newAttr->height);
  }
  if (mask & WIN_ATTR_BG_COLOR) {
    this->bgColor = newAttr->bgColor;
  }
  if (mask & WIN_ATTR_IS_RESIZABLE) {
    this->isResizable = newAttr->isResizable;
  }
  if (mask & WIN_ATTR_MAX_WIDTH) {
    this->maxWidth = newAttr->maxWidth;
  }
  if (mask & WIN_ATTR_MAX_HEIGHT) {
    this->maxHeight = newAttr->maxHeight;
  }
  if (mask & WIN_ATTR_MIN_WIDTH) {
    this->minWidth = newAttr->minWidth;
  }
  if (mask & WIN_ATTR_MIN_HEIGHT) {
    this->minHeight = newAttr->minHeight;
  }
}

void Window::selectInput(Client *client, unsigned int mask) {
  if (client == this->getCreator()) {
    this->eventMask = mask;
  } else {
    unsigned int i;
    for (i = 0; i < this->otherClients.size(); i++) {
      if (this->otherClients[i].client == client) {
        this->otherClients[i].mask = mask;
        break;
      }
   }
    if (i == this->otherClients.size()) { // Not found, add it
      this->otherClients.push_back(OtherClient(client, mask));
    }
  }
}

void Window::deliverEvent(PronEvent *e, unsigned int size) {
  unsigned int eventMask = PRON_EVENTMASK(e->type);
  
  // Deliver to creator
  if (this != this->getScreen()->tree->getRoot() && (this->eventMask & eventMask)) {
    this->getCreator()->send(e, size);
  }

  // Deliver to other clients
  for (unsigned int i = 0; i < this->otherClients.size(); i++) {
    if (this->otherClients[i].mask & eventMask) {
      this->otherClients[i].client->send(e, size);
    }
  }
}

void Window::deliverWindowEvent(PronEvent *e, unsigned int size) {
  this->deliverEvent(e, size);
  if (this->parent) {
    this->parent->deliverEvent(e, size);
  }
}

void Window::deliverDeviceEvent(PronEvent *e, unsigned int size) {
  unsigned int eventMask = PRON_EVENTMASK(e->type);

  // Set the event window
  e->window = this->getId();

  this->deliverEvent(e, size);

  if (this->parent && !(this->dontPropagateMask & eventMask)) {
    this->parent->deliverDeviceEvent(e, size);
  }
}

void Window::raise() {
  // Do nothing if we are the root window or already the last child of our parent
  if (this->parent == NULL || this->parent->lastChild == this) {
    return;
  }

  Window *sibling = this->nextSibling;
  bool overlap = false;
  for (; sibling != NULL; sibling = sibling->nextSibling) {
    overlap = overlap || this->overlaps(sibling);
  }

  if (this->prevSibling == NULL) {
    this->parent->firstChild = this->nextSibling;
  } else {
    this->prevSibling->nextSibling = this->nextSibling;  
  }
  if (this->nextSibling != NULL) {
    this->nextSibling->prevSibling = this->prevSibling;
  }
  this->prevSibling = this->parent->lastChild;
  this->nextSibling = NULL;
  this->parent->lastChild->nextSibling = this;
  this->parent->lastChild = this;

  if (overlap) {
    this->exposeArea(0, 0, this->getWidth(), this->getHeight());
  }
}

bool Window::overlaps(Window *w) {
  return !(w->x > this->x + this->getWidth() || w->y > this->y + this->getHeight() || w->x + w->getWidth() < this->x || w->y + w->getHeight() < this->y);
}

void Window::exposeArea(int x, int y, int width, int height) {
  this->clear(x, y, width, height);

  for (Window *child = this->firstChild; child != NULL; child = child->nextSibling) {
    child->exposeArea(x - child->x, y - child->y, this->getWidth(), this->getHeight());
  }
}

bool Window::contains(int x, int y) {
  return this->getX() <= x &&
      this->getY() <= y &&
      this->getX() + this->getWidth() > x &&
      this->getY() + this->getHeight() > y;
}

int Window::getX() {
  return this->x;
}

void Window::setX(int x) {
  this->x = x;
}

int Window::getY(){
  return this->y;
}

void Window::setY(int y) {
  this->y = y;
}

void Window::reparent(Window *w) {
  if (this->prevSibling != NULL) {
    this->prevSibling->nextSibling = this->nextSibling;
  } else {
    this->parent->firstChild = this->nextSibling;
  }
  if (this->nextSibling != NULL) {
    this->nextSibling->prevSibling = this->prevSibling;
  } else {
    this->parent->lastChild = this->prevSibling;
  }

  if (w->lastChild != NULL) {
    w->lastChild->nextSibling = this;
    this->prevSibling = w->lastChild;
    w->lastChild = this;
  } else {
    w->firstChild = this;
    w->lastChild = this;
    this->prevSibling = NULL;
  }
  this->nextSibling = NULL;
  
  this->parent = w;
}

void Window::destroy() {
  printf("window::destroy(%d)\n", this->getId());
  this->unmap();
  // We remove the window from the tree 
  if (this->prevSibling != NULL) {
    this->prevSibling->nextSibling = this->nextSibling;
  } else {
    this->parent->firstChild = this->nextSibling;
  }
  if (this->nextSibling != NULL) {
    this->nextSibling->prevSibling = this->prevSibling;
  } else {
    this->parent->lastChild = this->prevSibling;
  }
  this->getScreen()->destroy(this);
  delete this;
}

void Window::move(int dx, int dy) {
  this->unmap();

  for (WindowsTree::IteratorBFS it = this->getScreen()->tree->beginBFS(this); it != this->getScreen()->tree->endBFS(); it++) {
    it->x += dx;
    it->y += dy;
  }

  this->map();
}

void Window::moveTo(int x, int y) {
  this->move(x - this->x, y - this->y);
}

void Window::resize(int width, int height) {
  this->unmap();
  this->setWidth(width);
  this->setHeight(height);
  this->map();

  // Send resize event
  EventResizeWindow eventResizeWindow(width, height);
  this->deliverWindowEvent(&eventResizeWindow, sizeof(eventResizeWindow));
}

void Window::copyArea(int dstX, int dstY, Drawable *d, int srcX, int srcY, int width, int height) {
  // XXX : Bourrin à revoir (problème de depth et de byte per pixel de la pixmap et de l'écran)
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      this->setPixel(x + dstX, y + dstY, d->getPixel(x + srcX, y + srcY));
    }
  }
}
