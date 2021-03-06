#include <client.h>
#include <window.h>
#include <screen.h>
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <pronlib_enums.h>

Window::Window(Screen *screen, int id, Client *creator, Window *parent, int x, int y, int width, int height) {
  this->screen = screen;

  this->id = id;
  this->creator = creator;

  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  this->eventMask = 0;
  this->dontPropagateMask = 0;

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
}

void Window::reduce(int &x, int &y, int &width, int &height) {
  if (x < 0) {
    x = 0;
    width += x;
  }

  if (y < 0) {
    y = 0;
    height += y;
  }

  if (x + width > this->width) {
    width = this->width - x;
  }

  if (y + height > this->height) {
    height = this->height - y;
  }
}

/*short Window::getCreator() {
  return this->id >> 16;  
}*/

Client* Window::getCreator() {
  return this->creator;
}

void Window::drawPoint(int x, int y) {
  this->screen->drawPoint(this->x + x, this->y + y);
}

void Window::drawLine(int x1, int y1, int x2, int y2) {
  this->screen->drawLine(this->x + x1, this->y + y1, this->x + x2, this->y + y2);
}

void Window::drawRect(int x1, int y1, int width, int height) {
  this->screen->drawRect(this->x + x1, this->y + y1, width, height);
}

void Window::fillRectangle(int x1, int y1, int width, int height) {
  this->screen->fillRectangle(this->x + x1, this->y + y1, width, height);
}

void Window::drawCircle(int x, int y, int radius) {
  this->screen->drawCircle(this->x + x, this->y + y, radius); 
}

void Window::fillCircle(int x, int y, int radius) {
  this->screen->fillCircle(this->x + x, this->y + y, radius); 
}

void Window::clear(int x, int y, int width, int height) {
  this->reduce(x, y, width, height);
  
  color_t oldFg = this->screen->gc.fg;
  COLOR(this->screen->gc.fg, 24).r = (this->id >> 16) << 3;
  COLOR(this->screen->gc.fg, 24).g = (this->id >> 16) << 3;
  COLOR(this->screen->gc.fg, 24).b = (this->id >> 16) << 3;
  this->screen->fillRectangle(this->x + x, this->y + y, width, height);
  this->screen->gc.fg = oldFg;

  // Send exposure event
  EventExpose expose(this->id, x, y, width, height);
  this->deliverEvent(&expose, sizeof(expose));
}

void Window::clear() {
  this->clear(0, 0, width, height);
}

PronWindowAttributes Window::getAttributes() {
  PronWindowAttributes attr ;
  attr.x = this->x;
  attr.y = this->y;
  attr.width = this->width;
  attr.height = this->height;

  return attr;
}

void Window::setAttributes(PronWindowAttributes *newAttr, unsigned int mask) {
  if (mask & WIN_ATTR_X) {
    this->x = newAttr->x;
  }
  if (mask & WIN_ATTR_Y) {
    this->y = newAttr->y;
  }
  if (mask & WIN_ATTR_WIDTH) {
    this->width = newAttr->width;
  }
  if (mask & WIN_ATTR_HEIGHT) {
    this->height = newAttr->height;
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
  if (this != screen->root && (this->eventMask & eventMask)) {
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
 
  this->deliverEvent(e, size);

  if (this->parent && !(this->dontPropagateMask & eventMask)) {
    this->parent->deliverDeviceEvent(e, size);
  }
}

void Window::raise() {
  if (this->parent->lastChild == this) {
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
    this->exposeArea(this->x, this->y, this->width, this->height);
  }
}

bool Window::overlaps(Window *w) {
  return !(w->x > this->x + this->width || w->y > this->y + this->height || w->x + w->width < this->x || w->y + w->height < this->y);
}

void Window::exposeArea(int x, int y, int width, int height) {
  this->clear(x, y, width, height);

  for (Window *child = this->firstChild; child != NULL; child = child->nextSibling) {
    child->exposeArea(x - child->x, y - child->y, width, height);
  }
}