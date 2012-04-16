#include <client.h>
#include <window.h>
#include <screen.h>
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <pronlib_enums.h>
#include <algorithm>
#include <mouse.h>
#include <drawable.h>

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

void Window::unmap() {
  // Can't unmap root window
  if (this->parent == NULL) {
    return;
  }

  // Unmap all children
  for (Window *child = this->firstChild; child != NULL; child = child->nextSibling) {
    child->unmap();
  }

  this->mapped = false;

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

  // TODO: check if we were clipwin/mousewin/focuswin
  if (this->getScreen()->getClipWin() == this) {
    this->getScreen()->setClipWin(NULL);
  }

  if (this->getScreen()->getMouseWin() == this) {
    Mouse::getInstance()->updateMouseWin();
  }
}

void Window::map() {
  this->mapped = true;

  // Clear the window and send exposure event
  //this->clear();

  // Map all children
  for (Window *child = this->firstChild; child != NULL; child = child->nextSibling) {
    child->map();
  }

  // TODO: update clipwin/mousewin/focuswin
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
  delete this;
}

void Window::move(int dx, int dy) {
  // TODO Gestion sous-filles + clean
  this->unmap();
  this->x += dx;
  this->y += dy;
  for (Window *currentChild = this->firstChild; currentChild != NULL; currentChild = currentChild->nextSibling) {
    currentChild->x += dx;
    currentChild->y += dy;
  }
  this->map();
}

void Window::moveTo(int x, int y) {
  // TODO Gestion sous-filles + clean
  this->unmap();
  int xMove = x - this->x;
  int yMove = y - this->y;
  this->x = x;
  this->y = y;
  for (Window *currentChild = this->firstChild; currentChild != NULL; currentChild = currentChild->nextSibling) {
    currentChild->x += xMove;
    currentChild->y += yMove;
  }
  this->map();
}

void Window::resize(int width, int height) {
  if (this->isResizable) {
    this->unmap();
    this->setWidth(width);
    this->setHeight(height);
    this->map();

  // Send resize event
    EventResizeWindow eventResizeWindow(width, height);
    this->deliverWindowEvent(&eventResizeWindow, sizeof(eventResizeWindow));
  }
}
