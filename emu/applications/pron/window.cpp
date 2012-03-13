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
  COLOR(this->bgColor, 24).r = 0;
  COLOR(this->bgColor, 24).g = 0;
  COLOR(this->bgColor, 24).b = 0;
  this->height = height;
  this->eventMask = 0;
  this->dontPropagateMask = 0;
  this->mapped = false;

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

void Window::unmap() {
  // Can't unmap root window
  if (this->parent == NULL) {
    return;
  }

  // Unmap all children
  for (Window *child = this->firstChild; child != NULL; child = child->nextSibling) {
    child->unmap();
  }

  if (this->parent->mapped) {
    // Clear the area of the parent window occupied by this window and send exposure event
    this->parent->clear(this->x, this->y, this->width, this->height);

    // Redraw covered lower siblings
    for (Window *sib = this->prevSibling; sib != NULL; sib = sib->prevSibling) {
      if (this->overlaps(sib)) {
        sib->clear(this->x, this->y, this->width, this->height);
      }
    }
  }

  // TODO: check if we were clipwin/mousewin/focuswin

  this->mapped = false;
}

void Window::map() {
  this->mapped = true;

  // Clear the window and send exposure event
  this->clear();

  // Map all children
  for (Window *child = this->firstChild; child != NULL; child = child->nextSibling) {
    child->map();
  }

  // TODO: update clipwin/mousewin/focuswin
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
  COLOR(this->screen->gc.fg, 24).r = COLOR(this->bgColor, 24).r;
  COLOR(this->screen->gc.fg, 24).g = COLOR(this->bgColor, 24).g;
  COLOR(this->screen->gc.fg, 24).b = COLOR(this->bgColor, 24).b;
  this->screen->setClipWindow(this);
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
  attr.bgColor = this->bgColor;

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
  if (mask & WIN_ATTR_BG_COLOR) {
    this->bgColor = newAttr->bgColor;
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
    this->exposeArea(0, 0, this->width, this->height);
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

bool Window::contains(int x, int y){
  return this->getX() <= x &&
  this->getY() <= y &&
  this->getX() + this->getWidth() > x &&
  this->getY() + this->getHeight() > y;
}

int Window::getX(){
  return this->y;
}

void Window::setX(int x){
  this->x = x;
}

int Window::getY(){
  return this->y;
}

void Window::setY(int y){
  this->y = y;
}

int Window::getWidth(){
  return this->width;
}

void Window::setWidth(int width){
  this->width = width;
}

int Window::getHeight(){
  return this->height;
}

void Window::setHeight(int height){
  this->height = height;
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
