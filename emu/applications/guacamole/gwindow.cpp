#include <gwindows_manager.h>
#include <cstdio>

int buttonSize = 15;

GWindow::GWindow (Window w, const PronWindowAttributes & attributes, bool decorate, Display *display) {
  this->window      = w;
  this->attributes  = attributes;
  this->display     = display;
  this->isMaximised = false;

  if (decorate) {
    this->parent = pronCreateWindow(display, display->rootWindow,
      this->attributes.x - buttonSize, this->attributes.y - buttonSize,
      this->attributes.width + 2*buttonSize, this->attributes.height + 2*buttonSize);
    
    // Abonnement aux évènements souris de la fenêtre de décoration
    pronSelectInput(display, this->parent,
      PRON_EVENTMASK(EV_MOUSE_BUTTON) | PRON_EVENTMASK(EV_EXPOSE));
    
    pronReparentWindow(display, w, this->parent);
    pronDontPropagateEvent(display,w,PRON_EVENTMASK(EV_MOUSE_BUTTON));

    // registering for any EV_DESTROY_WINDOW event
    pronSelectInput(display, w,
      PRON_EVENTMASK(EV_DESTROY_WINDOW) | PRON_EVENTMASK(EV_MOUSE_BUTTON));
    pronGetWindowAttributes(display, this->parent, &(this->parentAttributes));

    // Fond rouge à la fenêtre de déco provisoire
    COLOR(this->parentAttributes.bgColor, 24).r = 255;
    COLOR(this->parentAttributes.bgColor, 24).g = 0;
    COLOR(this->parentAttributes.bgColor, 24).b = 0;
    pronSetWindowAttributes(display, this->parent, this->parentAttributes, WIN_ATTR_BG_COLOR);
    pronDontPropagateEvent(display,this->parent,PRON_EVENTMASK(EV_MOUSE_BUTTON));

    // Adding the close button
    this->closeButton = pronCreateWindow(display, this->parent,
      this->attributes.x + this->attributes.width, this->attributes.y - buttonSize,
      buttonSize, buttonSize);
    PronWindowAttributes closeButtonAttributes;
    COLOR(closeButtonAttributes.bgColor, 24).r = 0;
    COLOR(closeButtonAttributes.bgColor, 24).g = 255;
    COLOR(closeButtonAttributes.bgColor, 24).b = 0;
    pronSetWindowAttributes(display, this->closeButton, closeButtonAttributes, WIN_ATTR_BG_COLOR);
    pronSelectInput(display, this->closeButton, PRON_EVENTMASK(EV_MOUSE_BUTTON));
    pronDontPropagateEvent(display,this->closeButton,PRON_EVENTMASK(EV_MOUSE_BUTTON));

    PronGCValues values;
    COLOR(values.fg, 24).r = 0;
    COLOR(values.fg, 24).g = 0;
    COLOR(values.fg, 24).b = 0;
    COLOR(values.bg, 24).r = 0;
    COLOR(values.bg, 24).g = 0;
    COLOR(values.bg, 24).b = 0;
    this->closeButtonGC = pronCreateGC(this->display, values, GC_VAL_FG | GC_VAL_BG);

    // Adding the resize button
    this->resizeButton = pronCreateWindow(display, this->parent,
      this->attributes.x + this->attributes.width,
      this->attributes.y + this->attributes.height,
      buttonSize, buttonSize);
    PronWindowAttributes resizeButtonAttributes;
    COLOR(closeButtonAttributes.bgColor, 24).r = 0;
    COLOR(closeButtonAttributes.bgColor, 24).g = 0;
    COLOR(closeButtonAttributes.bgColor, 24).b = 150;
    pronSetWindowAttributes(display, this->resizeButton, resizeButtonAttributes, WIN_ATTR_BG_COLOR);
    pronSelectInput(display, this->resizeButton, PRON_EVENTMASK(EV_MOUSE_BUTTON));
    pronDontPropagateEvent(display,this->resizeButton,PRON_EVENTMASK(EV_MOUSE_BUTTON));

    // Adding the maximise button
    this->maximiseButton = pronCreateWindow(display, this->parent,
      this->attributes.x + this->attributes.width - buttonSize, this->attributes.y - buttonSize,
      buttonSize, buttonSize);
    PronWindowAttributes maximiseButtonAttributes;
    COLOR(maximiseButtonAttributes.bgColor, 24).r = 30;
    COLOR(maximiseButtonAttributes.bgColor, 24).g = 190;
    COLOR(maximiseButtonAttributes.bgColor, 24).b = 255;
    pronSetWindowAttributes(display, this->maximiseButton, maximiseButtonAttributes, WIN_ATTR_BG_COLOR);
    pronSelectInput(display, this->maximiseButton, PRON_EVENTMASK(EV_MOUSE_BUTTON));
    pronDontPropagateEvent(display,this->maximiseButton,PRON_EVENTMASK(EV_MOUSE_BUTTON));


    this->decorate();
  }
}

bool GWindow::hasDecoration() {
  return (parent != 0);
}

bool GWindow::overlaps(GWindow *gw) {
  bool res =
      // Si coin en haut à gauche dedans
      (this->parentAttributes.x >= this->parentAttributes.x
      && this->parentAttributes.y >= this->parentAttributes.y
      && this->parentAttributes.x < this->parentAttributes.x + this->parentAttributes.width
      && this->parentAttributes.y < this->parentAttributes.y + this->parentAttributes.height)
    || // SI coin en haut à droite
      (this->parentAttributes.x + this->parentAttributes.width >= this->parentAttributes.x
      && this->parentAttributes.y >= this->parentAttributes.y
      && this->parentAttributes.x + this->parentAttributes.width < this->parentAttributes.x + this->parentAttributes.width
      && this->parentAttributes.y < this->parentAttributes.y + this->parentAttributes.height)
    || // SI coin en bas à gauche
      (this->parentAttributes.x >= this->parentAttributes.x
      && this->parentAttributes.y + this->parentAttributes.height >= this->parentAttributes.y
      && this->parentAttributes.x < this->parentAttributes.x + this->parentAttributes.width
      && this->parentAttributes.y + this->parentAttributes.height < this->parentAttributes.y + this->parentAttributes.height)
    || // SI coin en bas à droite
      (this->parentAttributes.x + this->parentAttributes.width >= this->parentAttributes.x
      && this->parentAttributes.y + this->parentAttributes.height >= this->parentAttributes.y
      && this->parentAttributes.x + this->parentAttributes.width < this->parentAttributes.x + this->parentAttributes.width
      && this->parentAttributes.y + this->parentAttributes.height < this->parentAttributes.y + this->parentAttributes.height);
  return res;
}


void GWindow::decorate() {
  if (this->hasDecoration()) {
    pronDrawLine(display, this->closeButton, this->closeButtonGC, 0, 1, buttonSize-2, buttonSize-1);
    pronDrawLine(display, this->closeButton, this->closeButtonGC, 0, 0, buttonSize-1, buttonSize-1);
    pronDrawLine(display, this->closeButton, this->closeButtonGC, 0, buttonSize-1, buttonSize-1, 0);
    pronDrawLine(display, this->closeButton, this->closeButtonGC, 1, buttonSize-1, buttonSize-1, 1);
    if (!this->isMaximised) {
      pronDrawLine(display, this->maximiseButton, this->closeButtonGC, 2, 2, 2, buttonSize-3);
      pronDrawLine(display, this->maximiseButton, this->closeButtonGC, 2, 2, buttonSize-3, 2);
      pronDrawLine(display, this->maximiseButton, this->closeButtonGC, buttonSize-3, 2, buttonSize-3, buttonSize-3);
      pronDrawLine(display, this->maximiseButton, this->closeButtonGC, 2, buttonSize-3, buttonSize-3, buttonSize-3);
      pronDrawLine(display, this->maximiseButton, this->closeButtonGC, 3, 3, 3, buttonSize-4);
      pronDrawLine(display, this->maximiseButton, this->closeButtonGC, 3, 3, buttonSize-4, 3);
      pronDrawLine(display, this->maximiseButton, this->closeButtonGC, buttonSize-4, 3, buttonSize-4, buttonSize-4);
      pronDrawLine(display, this->maximiseButton, this->closeButtonGC, 3, buttonSize-4, buttonSize-4, buttonSize-4);
    }
  }
}


void GWindow::resize(int width, int height) {
  if (!this->isMaximised) {
    pronResizeWindow(display, this->parent, width, height);
    pronMoveWindow(display, this->closeButton,
      width - this->parentAttributes.width, 0);
    pronMoveWindow(display, this->maximiseButton,
      width - this->parentAttributes.width, 0);
    pronMoveWindow(display, this->resizeButton,
      width - this->parentAttributes.width, height - this->parentAttributes.height);
    this->parentAttributes.width  = width;
    this->parentAttributes.height = height;
    this->attributes.width        = width  - 2*buttonSize;
    this->attributes.height       = height - 2*buttonSize;
    pronResizeWindow(display, this->window, this->attributes.width,
      this->attributes.height);
  }
}


void GWindow::move(int xMove, int yMove) {
  if (!this->isMaximised) {
    pronMoveWindow(display, this->parent, xMove, yMove);
    this->parentAttributes.x += xMove;
    this->parentAttributes.y += yMove;
    this->attributes.x       += xMove;
    this->attributes.y       += yMove;
  }
}


void GWindow::maximise() {
  if (!this->isMaximised) {
    // On en profite pour mettre tous les attributs à jour
    pronGetWindowAttributes(display, this->window, &this->attributes);
    pronGetWindowAttributes(display, this->parent, &this->parentAttributes);
    this->oldParentAttributes = this->parentAttributes;
    PronWindowAttributes rootAttributes;
    pronGetWindowAttributes(display, display->rootWindow, &rootAttributes);
    pronMoveWindowTo(display, this->parent, 0, 0);
    this->resize(rootAttributes.width, rootAttributes.height);
    // TODO unmap de resizewindow
    this->isMaximised = true;
  } else {
    this->isMaximised = false;
    this->resize(this->oldParentAttributes.width, this->oldParentAttributes.height);
    pronMoveWindowTo(display, this->parent, this->oldParentAttributes.x, this->oldParentAttributes.y);
    // On en profite pour mettre tous les attributs à jour
    pronGetWindowAttributes(display, this->window, &this->attributes);
    pronGetWindowAttributes(display, this->parent, &this->parentAttributes);
  }
}


void GWindow::destroy() {
  pronDestroyWindow(display,this->parent);
  pronFreeGC(display, closeButtonGC);
}