#include <gwindows_manager.h>
#include <cstdio>


int buttonSize = 15;

GWindow::GWindow (Window w, const PronWindowAttributes & attributes, bool decorate, Display *display) {
  this->window     = w;
  this->display    = display;
  this->attributes = attributes;
  if (decorate) {
    this->parent = pronCreateWindow(display, display->rootWindow,
      attributes.x - buttonSize, attributes.y - buttonSize,
      attributes.width + 2*buttonSize, attributes.height + 2*buttonSize);
    
    pronMapWindow(display, this->parent);

    // Abonnement aux évènements souris de la fenêtre de décoration
    pronSelectInput(display, this->parent,
      PRON_EVENTMASK(EV_MOUSE_BUTTON) | PRON_EVENTMASK(EV_EXPOSE));
    
    pronReparentWindow(display, this->window, this->parent);
    pronDontPropagateEvent(display,this->window,PRON_EVENTMASK(EV_MOUSE_BUTTON) | PRON_EVENTMASK(EV_EXPOSE));
    
    this->attributes = attributes;
    // registering for any EV_DESTROY_WINDOW event
    pronSelectInput(display, this->window,
      PRON_EVENTMASK(EV_DESTROY_WINDOW) | PRON_EVENTMASK(EV_MOUSE_BUTTON));
    pronGetWindowAttributes(display, this->parent, &(this->parentAttributes));

    // Fond rouge à la fenêtre de déco provisoire
    COLOR(this->parentAttributes.bgColor, 24).r = 255;
    COLOR(this->parentAttributes.bgColor, 24).g = 0;
    COLOR(this->parentAttributes.bgColor, 24).b = 0;
    pronSetWindowAttributes(display, this->parent, this->parentAttributes, WIN_ATTR_BG_COLOR);
    pronDontPropagateEvent(display,this->parent,
      PRON_EVENTMASK(EV_MOUSE_BUTTON));

    // Adding the close button
    /*this->closeButton = pronCreateWindow(display, this->parent,
      attributes.x + attributes.width, attributes.y - buttonSize,
      buttonSize, buttonSize);
    PronWindowAttributes closeButtonAttributes;
    COLOR(closeButtonAttributes.bgColor, 24).r = 0;
    COLOR(closeButtonAttributes.bgColor, 24).g = 255;
    COLOR(closeButtonAttributes.bgColor, 24).b = 0;
    pronSetWindowAttributes(display, this->closeButton, closeButtonAttributes, WIN_ATTR_BG_COLOR);
    pronSelectInput(display, this->closeButton, PRON_EVENTMASK(EV_MOUSE_BUTTON));
    pronDontPropagateEvent(display,this->closeButton,PRON_EVENTMASK(EV_MOUSE_BUTTON));*/

    PronGCValues values;
    COLOR(values.fg, 24).r = 0;
    COLOR(values.fg, 24).g = 255;
    COLOR(values.fg, 24).b = 0;
    COLOR(values.bg, 24).r = 0;
    COLOR(values.bg, 24).g = 0;
    COLOR(values.bg, 24).b = 0;
    this->closeButtonGC = pronCreateGC(this->display, values, GC_VAL_FG | GC_VAL_BG);

    // Adding the resize button
    COLOR(values.fg, 24).r = 0;
    COLOR(values.fg, 24).g = 0;
    COLOR(values.fg, 24).b = 255;
    COLOR(values.bg, 24).r = 0;
    COLOR(values.bg, 24).g = 0;
    COLOR(values.bg, 24).b = 0;
    this->resizeButtonGC = pronCreateGC(this->display, values, GC_VAL_FG | GC_VAL_BG);

    /*this->resizeButton = pronCreateWindow(display, this->parent,
      attributes.x + attributes.width,
      attributes.y + attributes.height,
      buttonSize, buttonSize);
    PronWindowAttributes resizeButtonAttributes;
    COLOR(resizeButtonAttributes.bgColor, 24).r = 0;
    COLOR(resizeButtonAttributes.bgColor, 24).g = 0;
    COLOR(resizeButtonAttributes.bgColor, 24).b = 255;
    pronSetWindowAttributes(display, this->resizeButton, resizeButtonAttributes, WIN_ATTR_BG_COLOR);
    pronSelectInput(display, this->resizeButton, PRON_EVENTMASK(EV_MOUSE_BUTTON));
    pronDontPropagateEvent(display,this->resizeButton,PRON_EVENTMASK(EV_MOUSE_BUTTON));*/
  }
}

bool GWindow::hasDecoration() {
  return (parent == 0);
}

bool GWindow::overlaps(GWindow *gw) {
  bool res =
      // Si coin en haut à gauche dedans
      (this->parentAttributes.x >= gw->parentAttributes.x
      && this->parentAttributes.y >= gw->parentAttributes.y
      && this->parentAttributes.x < gw->parentAttributes.x + gw->parentAttributes.width
      && this->parentAttributes.y < gw->parentAttributes.y + gw->parentAttributes.height)
    || // SI coin en haut à droite
      (this->parentAttributes.x + this->parentAttributes.width >= gw->parentAttributes.x
      && this->parentAttributes.y >= gw->parentAttributes.y
      && this->parentAttributes.x + this->parentAttributes.width < gw->parentAttributes.x + gw->parentAttributes.width
      && this->parentAttributes.y < gw->parentAttributes.y + gw->parentAttributes.height)
    || // SI coin en bas à gauche
      (this->parentAttributes.x >= gw->parentAttributes.x
      && this->parentAttributes.y + this->parentAttributes.height >= gw->parentAttributes.y
      && this->parentAttributes.x < gw->parentAttributes.x + gw->parentAttributes.width
      && this->parentAttributes.y + this->parentAttributes.height < gw->parentAttributes.y + gw->parentAttributes.height)
    || // SI coin en bas à droite
      (this->parentAttributes.x + this->parentAttributes.width >= gw->parentAttributes.x
      && this->parentAttributes.y + this->parentAttributes.height >= gw->parentAttributes.y
      && this->parentAttributes.x + this->parentAttributes.width < gw->parentAttributes.x + gw->parentAttributes.width
      && this->parentAttributes.y + this->parentAttributes.height < gw->parentAttributes.y + gw->parentAttributes.height);
  return res;
}


void GWindow::decorate() {
  // A décommenter quand on aura moins de expose event
  //pronClearWindow(display, this->parent);
  pronFillRectangle(display, this->parent, this->closeButtonGC, this->parentAttributes.width - 15,0,15,15);
  pronFillRectangle(display, this->parent, this->resizeButtonGC, this->parentAttributes.width - 15,
    this->parentAttributes.height - 15,15,15);

}

bool GWindow::isCloseButton(int x, int y) {
  return (this->parentAttributes.x + this->parentAttributes.width > x
    && this->parentAttributes.x + this->parentAttributes.width - 15 <= x
    && this->parentAttributes.y < y
    && this->parentAttributes.y + 15 >= y);
}

bool GWindow::isResizeButton(int x, int y) {
  return (this->parentAttributes.x + this->parentAttributes.width > x
    && this->parentAttributes.x + this->parentAttributes.width - 15 <= x
    && this->parentAttributes.y + this->parentAttributes.height > y
    && this->parentAttributes.y + this->parentAttributes.height - 15 <= y);
}