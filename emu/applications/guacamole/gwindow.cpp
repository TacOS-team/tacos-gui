#include <stdio.h>
#include <unistd.h>

#include <gwindows_manager.h>

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
    this->parentAttributes.bgColor = Color(255, 0, 0);
    pronSetWindowAttributes(display, this->parent, this->parentAttributes, WIN_ATTR_BG_COLOR);
    pronDontPropagateEvent(display,this->parent,PRON_EVENTMASK(EV_MOUSE_BUTTON));

    // Adding the close button
    this->closeButton = pronCreateWindow(display, this->parent,
      this->attributes.x + this->attributes.width, this->attributes.y - buttonSize,
      buttonSize, buttonSize);
    PronWindowAttributes closeButtonAttributes;
    closeButtonAttributes.bgColor = Color(0, 255, 0);
    pronSetWindowAttributes(display, this->closeButton, closeButtonAttributes, WIN_ATTR_BG_COLOR);
    pronSelectInput(display, this->closeButton, PRON_EVENTMASK(EV_MOUSE_BUTTON));
    pronDontPropagateEvent(display,this->closeButton,PRON_EVENTMASK(EV_MOUSE_BUTTON));

    PronGCValues values;
    values.fg = Color(0, 0, 0);
    values.bg = Color(0, 0, 0);
    this->closeButtonGC = pronCreateGC(this->display, values, GC_VAL_FG | GC_VAL_BG);

    values.fg = Color(255, 0, 0);
    values.bg = Color(0, 0, 0);
    this->backgroundParentGC = pronCreateGC(this->display, values, GC_VAL_FG | GC_VAL_BG);

    values.fg = Color(0, 255, 0);
    values.bg = Color(0, 0, 0);
    this->backgroundCloseButtonGC = pronCreateGC(this->display, values, GC_VAL_FG | GC_VAL_BG);

    values.fg = Color(30, 190, 255);
    values.bg = Color(0, 0, 0);
    this->backgroundMaximiseButtonGC = pronCreateGC(this->display, values, GC_VAL_FG | GC_VAL_BG);

    values.fg = Color(0, 0, 200);
    values.bg = Color(0, 0, 0);
    this->backgroundResizeButtonGC = pronCreateGC(this->display, values, GC_VAL_FG | GC_VAL_BG);

    // Adding the resize button
    this->resizeButton = pronCreateWindow(display, this->parent,
      this->attributes.x + this->attributes.width,
      this->attributes.y + this->attributes.height,
      buttonSize, buttonSize);
    PronWindowAttributes resizeButtonAttributes;
    closeButtonAttributes.bgColor = Color(0, 0, 150);
    pronSetWindowAttributes(display, this->resizeButton, resizeButtonAttributes, WIN_ATTR_BG_COLOR);
    pronSelectInput(display, this->resizeButton, PRON_EVENTMASK(EV_MOUSE_BUTTON));
    pronDontPropagateEvent(display,this->resizeButton,PRON_EVENTMASK(EV_MOUSE_BUTTON));

    // Adding the maximise button
    this->maximiseButton = pronCreateWindow(display, this->parent,
      this->attributes.x + this->attributes.width - buttonSize, this->attributes.y - buttonSize,
      buttonSize, buttonSize);
    PronWindowAttributes maximiseButtonAttributes;
    maximiseButtonAttributes.bgColor = Color(30, 190, 255);
    pronSetWindowAttributes(display, this->maximiseButton, maximiseButtonAttributes, WIN_ATTR_BG_COLOR);
    pronSelectInput(display, this->maximiseButton, PRON_EVENTMASK(EV_MOUSE_BUTTON));
    pronDontPropagateEvent(display,this->maximiseButton,PRON_EVENTMASK(EV_MOUSE_BUTTON));

    GWindowsManager::getInstance()->initWindowPosition(this);
    pronMoveWindow(display, this->parent, this->parentAttributes.x, this->parentAttributes.y);

    // Map all windows
    pronMapWindow(display, this->parent);
    pronMapWindow(display, this->closeButton);
    pronMapWindow(display, this->resizeButton);
    pronMapWindow(display, this->maximiseButton);

    GWindowsManager::getInstance()->addGWindow(this);
    //this->decorate();
  }
}

bool GWindow::hasDecoration() {
  return (parent != 0);
}

bool GWindow::overlaps(GWindow *gw) {
  bool dehors =
    this->parentAttributes.x > gw->parentAttributes.x + gw->parentAttributes.width ||
    this->parentAttributes.x + this->parentAttributes.width < gw->parentAttributes.x ||
    this->parentAttributes.y > gw->parentAttributes.y + gw->parentAttributes.height ||
    this->parentAttributes.y + this->parentAttributes.height < gw->parentAttributes.y;

  return !dehors;
}

void GWindow::decorate() {
  if (this->hasDecoration()) {
    // Dessin des fonds
    // Barre du haut
    pronFillRectangle(display, this->parent, this->backgroundParentGC, 0,0,
      this->parentAttributes.width - 2* buttonSize, buttonSize);
    
    char windowTitle[255];
    sprintf(windowTitle, "Window %x", this->window);
    int width, height;
    pronTextSize(display, display->defaultGC, windowTitle, strlen(windowTitle), &width, &height);
    printf("Writing %s (size %d %d)...\n", windowTitle, width, height);
    pronDrawText(display, this->parent, display->defaultGC, 2, 0, windowTitle, strlen(windowTitle));

    // Barre de gauche
    pronFillRectangle(display, this->parent, this->backgroundParentGC, 0,buttonSize,
      buttonSize, this->parentAttributes.height-buttonSize);
    // Barre du bas
    pronFillRectangle(display, this->parent, this->backgroundParentGC,
      buttonSize, this->parentAttributes.height - buttonSize,
      this->parentAttributes.width - buttonSize, buttonSize);
    // Barre de droite
    pronFillRectangle(display, this->parent, this->backgroundParentGC,
      this->parentAttributes.width - buttonSize, buttonSize,
      buttonSize, this->parentAttributes.height - 2*buttonSize);/**/
    // Bouton close
    pronFillRectangle(display, this->closeButton, this->backgroundCloseButtonGC, 0,0, buttonSize, buttonSize);
    // Bouton maximise
    pronFillRectangle(display, this->maximiseButton, this->backgroundMaximiseButtonGC, 0,0, buttonSize, buttonSize);

    pronDrawLine(display, this->closeButton, this->closeButtonGC,
      0, 1, buttonSize-2, buttonSize-1);
    pronDrawLine(display, this->closeButton, this->closeButtonGC,
      0, 0, buttonSize-1, buttonSize-1);
    pronDrawLine(display, this->closeButton, this->closeButtonGC,
      0, buttonSize-1, buttonSize-1, 0);
    pronDrawLine(display, this->closeButton, this->closeButtonGC,
      1, buttonSize-1, buttonSize-1, 1);
    if (!this->isMaximised) {
      // Trait du haut
      pronDrawLine(display, this->maximiseButton, this->closeButtonGC,
        2, 2, buttonSize-3, 2);
      pronDrawLine(display, this->maximiseButton, this->closeButtonGC,
        3, 3, buttonSize-4, 3);
      // Trait de gauche
      pronDrawLine(display, this->maximiseButton, this->closeButtonGC,
        2, 2, 2, buttonSize-3);
      // Trait de droite
      pronDrawLine(display, this->maximiseButton, this->closeButtonGC,
        buttonSize-3, 2, buttonSize-3, buttonSize-3);
      // Trait du bas
      pronDrawLine(display, this->maximiseButton, this->closeButtonGC,
        2, buttonSize-3, buttonSize-3, buttonSize-3);
      pronFillRectangle(display, this->resizeButton, this->backgroundResizeButtonGC, 0,0, buttonSize, buttonSize);
    } else {
      // devant gauche
      pronDrawLine(display, this->maximiseButton, this->closeButtonGC,
        2, 4, 2, buttonSize-3);
      // devant bas
      pronDrawLine(display, this->maximiseButton, this->closeButtonGC,
        2, buttonSize-3, buttonSize-5, buttonSize-3);
      // devant droite
      pronDrawLine(display, this->maximiseButton, this->closeButtonGC,
        buttonSize-5, 4, buttonSize-5, buttonSize-3);
      // devant haut
      pronDrawLine(display, this->maximiseButton, this->closeButtonGC,
        2, 4, buttonSize-5, 4);
      // derrière haut
      pronDrawLine(display, this->maximiseButton, this->closeButtonGC,
        4, 2, buttonSize-3, 2);
      // derrière droite
      pronDrawLine(display, this->maximiseButton, this->closeButtonGC,
        buttonSize-3, 2, buttonSize-3, buttonSize-8);
      // derrière bas
      pronDrawPoint(display, this->maximiseButton, this->closeButtonGC,
        buttonSize-4, buttonSize-8);
      // derrière gauche
      pronDrawPoint(display, this->maximiseButton, this->closeButtonGC,
        4, 3);

// 012345678901234
//0
//2    *********
//3    *       *
//4  ********* *
//5  *       * *
//6  *       * *
//7  *       ***
//8  *       *
//9  *       *
//0  *       *
//1  *       *
//2  *********
//3
//4

    }
  }
}


void GWindow::resize(int width, int height) {
  // We first retrieve the attributes of the window that has to be resized
  // so that we can know if the window is resizable
  PronWindowAttributes attr;
  pronGetWindowAttributes(this->display, this->window, &attr);
  if (attr.isResizable) {
    if (attr.maxWidth != -1 && attr.maxWidth <= width) {
      width = attr.maxWidth ;
    }
    if (attr.minWidth != -1 && attr.minWidth >= width ) {
      width = attr.minWidth;
    }  
    if ((attr.maxHeight != -1 && attr.maxHeight <= height) ) {
      height = attr.maxHeight ;
    }
    if (attr.minHeight != -1 && attr.minHeight >= height) {
      height = attr.minHeight;
    }
    printf("%d %d %d %d\n",attr.minHeight, attr.minWidth, height, width);
    if (!this->isMaximised){
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
  // We first retrieve the attributes of the window that has to be resized
  // so that we can know if the window is resizable
  PronWindowAttributes windowAttributes;
  pronGetWindowAttributes(this->display, this->window, &windowAttributes);
  if (windowAttributes.isResizable && windowAttributes.maxHeight == -1 && windowAttributes.maxWidth == -1){
    if (!this->isMaximised) {
      // On en profite pour mettre tous les attributs à jour
      this->attributes = windowAttributes;
      pronGetWindowAttributes(display, this->parent, &this->parentAttributes);
      this->oldParentAttributes = this->parentAttributes;
      pronUnmapWindow(display, this->parent);
      pronMoveWindowTo(display, this->parent, 0, 0);
      this->resize(GWindowsManager::getInstance()->getRootWindowAttributes().width,
        GWindowsManager::getInstance()->getRootWindowAttributes().height);
      // Puts the window on foreground
      this->raise();
      pronMapWindow(display, this->parent);
      /** @todo unmap de resizewindow */
      this->isMaximised = true;
    } else {
      this->isMaximised = false;
      pronUnmapWindow(display, this->parent);
      this->resize(this->oldParentAttributes.width, this->oldParentAttributes.height);
      pronMoveWindowTo(display, this->parent, this->oldParentAttributes.x, this->oldParentAttributes.y);
      pronMapWindow(display, this->parent);
      // On en profite pour mettre tous les attributs à jour
      this->attributes = windowAttributes;
      pronGetWindowAttributes(display, this->parent, &this->parentAttributes);
    }
  }
}


GWindow::~GWindow() {
  pronDestroyWindow(display, this->parent);
  pronFreeGC(display, closeButtonGC);
}


void GWindow::raise() {
  pronRaiseWindow(display, this->parent);
}
