#include <stdio.h>
#include <unistd.h>

#include <gwindows_manager.h>

int buttonSize = 9;
int buttonMargin = 4;
int borderSize = 1;
int titleBarSize = 22;

GWindow *raisedWindow = NULL;

extern Color activeTitleColor, inactiveTitleColor, borderColor;
extern GC whiteGC, blackGC, borderGC, activeTitleGC, inactiveTitleGC;

GWindow::GWindow (Window w, const PronWindowAttributes & attributes, bool decorate, Display *display) {
  this->window      = w;
  this->attributes  = attributes;
  this->display     = display;
  this->isMaximised = false;

  if (raisedWindow == NULL) {
    raisedWindow = this;
  }

  int decoratedX      = this->attributes.x;
  int decoratedY      = this->attributes.y;
  int decoratedWidth  = this->attributes.width  + 2 * borderSize;
  int decoratedHeight = this->attributes.height + titleBarSize + borderSize;

  if (decorate) {
    this->parent = pronCreateWindow(display, display->rootWindow,
        decoratedX, decoratedY, decoratedWidth, decoratedHeight);
    
    // Abonnement aux évènements souris de la fenêtre de décoration
    pronSelectInput(display, this->parent,
        PRON_EVENTMASK(EV_MOUSE_BUTTON) | PRON_EVENTMASK(EV_EXPOSE));
    
    pronReparentWindow(display, w, this->parent);
    pronMoveWindow(display, w, borderSize, titleBarSize);
    pronDontPropagateEvent(display, w, PRON_EVENTMASK(EV_MOUSE_BUTTON));

    // registering for any EV_DESTROY_WINDOW event
    pronSelectInput(display, w,
        PRON_EVENTMASK(EV_DESTROY_WINDOW) | PRON_EVENTMASK(EV_MOUSE_BUTTON));
    pronGetWindowAttributes(display, this->parent, &(this->parentAttributes));

    // Couleur de fond de la fenêtre de décoration
    this->parentAttributes.bgColor = borderColor;
    pronSetWindowAttributes(display, this->parent, this->parentAttributes, WIN_ATTR_BG_COLOR);
    pronDontPropagateEvent(display,this->parent,PRON_EVENTMASK(EV_MOUSE_BUTTON));

    // Adding the close button
    this->closeButton = pronCreateWindow(display, this->parent,
        decoratedX + decoratedWidth - borderSize - (buttonSize + buttonMargin), decoratedY + (titleBarSize - buttonSize) / 2,
        buttonSize, buttonSize);
    PronWindowAttributes closeButtonAttributes;
    closeButtonAttributes.bgColor = borderColor;
    pronSetWindowAttributes(display, this->closeButton, closeButtonAttributes, WIN_ATTR_BG_COLOR);
    pronSelectInput(display, this->closeButton, PRON_EVENTMASK(EV_MOUSE_BUTTON));
    pronDontPropagateEvent(display,this->closeButton,PRON_EVENTMASK(EV_MOUSE_BUTTON));

    // Adding the resize button
    this->resizeButton = pronCreateWindow(display, this->parent,
        decoratedX + decoratedWidth - borderSize - buttonSize, decoratedY + decoratedHeight - borderSize - buttonSize,
        buttonSize, buttonSize);
    PronWindowAttributes resizeButtonAttributes;
    closeButtonAttributes.bgColor = borderColor;
    pronSetWindowAttributes(display, this->resizeButton, resizeButtonAttributes, WIN_ATTR_BG_COLOR);
    pronSelectInput(display, this->resizeButton, PRON_EVENTMASK(EV_MOUSE_BUTTON));
    pronDontPropagateEvent(display,this->resizeButton,PRON_EVENTMASK(EV_MOUSE_BUTTON));

    // Adding the maximise button
    this->maximiseButton = pronCreateWindow(display, this->parent,
        decoratedX + decoratedWidth - borderSize - 2 * (buttonSize + 2 * buttonMargin) + buttonMargin, decoratedY + (titleBarSize - buttonSize) /2,
        buttonSize, buttonSize);
    PronWindowAttributes maximiseButtonAttributes;
    maximiseButtonAttributes.bgColor = borderColor;
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
    this->parentAttributes.x >= gw->parentAttributes.x + gw->parentAttributes.width ||
    this->parentAttributes.x + this->parentAttributes.width <= gw->parentAttributes.x ||
    this->parentAttributes.y >= gw->parentAttributes.y + gw->parentAttributes.height ||
    this->parentAttributes.y + this->parentAttributes.height <= gw->parentAttributes.y;

  return !dehors;
}

void GWindow::decorate() {
  GC titleGC = (this == raisedWindow) ? activeTitleGC : inactiveTitleGC;

  if (this->hasDecoration()) {
    // Dessin des fonds
    // Barre du haut
    pronFillRectangle(display, this->parent, titleGC,
        0, 0, this->parentAttributes.width, titleBarSize);
    pronDrawRect(display, this->parent, borderGC,
        0, 0, this->parentAttributes.width, titleBarSize);
   
    int textWidth, textHeight;
    pronTextSize(display, whiteGC, this->attributes.wm_title,
        strlen(this->attributes.wm_title), &textWidth, &textHeight);
    printf("Writing %s (size %d %d)...\n", this->attributes.wm_title,
        textWidth, textHeight);
    pronDrawText(display, this->parent, whiteGC,
        5, (titleBarSize - textHeight) / 2,
        this->attributes.wm_title, strlen(this->attributes.wm_title));

    // Barre de gauche
    pronFillRectangle(display, this->parent, borderGC,
        0, buttonSize, borderSize, this->parentAttributes.height - buttonSize);
    // Barre du bas
    pronFillRectangle(display, this->parent, borderGC,
        0, this->parentAttributes.height - borderSize,
        this->parentAttributes.width, borderSize);
    // Barre de droite
    pronFillRectangle(display, this->parent, borderGC,
        this->parentAttributes.width - borderSize, buttonSize,
        borderSize, this->parentAttributes.height - buttonSize);
    // Bouton close
    pronFillRectangle(display, this->closeButton, titleGC, 0,0, buttonSize, buttonSize);
    // Bouton maximise
    pronFillRectangle(display, this->maximiseButton, titleGC, 0,0, buttonSize, buttonSize);

    pronDrawLine(display, this->closeButton, whiteGC,
        1, 0, buttonSize - 1, buttonSize - 2);
    pronDrawLine(display, this->closeButton, whiteGC,
        0, 1, buttonSize - 2, buttonSize - 1);
    pronDrawLine(display, this->closeButton, whiteGC,
        0, buttonSize - 2, buttonSize - 2, 0);
    pronDrawLine(display, this->closeButton, whiteGC,
        1, buttonSize - 1, buttonSize - 1, 1);
    pronDrawLine(display, this->closeButton, whiteGC,
        0, 0, buttonSize - 1, buttonSize - 1);
    pronDrawLine(display, this->closeButton, whiteGC,
        0, buttonSize - 1, buttonSize - 1, 0);

    if (!this->isMaximised) {
      pronDrawRect(display, this->maximiseButton, whiteGC,
          0, 0, buttonSize, buttonSize);
      pronDrawLine(display, this->maximiseButton, whiteGC,
          0, 1, buttonSize, 1);
      pronFillRectangle(display, this->resizeButton, titleGC,
          0, 0, buttonSize, buttonSize);
    } else {
      // Devant
      pronDrawRect(display, this->maximiseButton, whiteGC,
          0, 3, 7, 6);
      // Double haut devant
      pronDrawLine(display, this->maximiseButton, whiteGC,
          0, 2, 6, 2);
      // Arrière haut
      pronDrawLine(display, this->maximiseButton, whiteGC,
          3, 0, 8, 0);
      // Arrière bas
      pronDrawLine(display, this->maximiseButton, whiteGC,
          7, 5, 8, 5);
      // Arrière gauche
      pronDrawLine(display, this->maximiseButton, whiteGC,
          3, 0, 3, 2);
      // Arrière droite
      pronDrawLine(display, this->maximiseButton, whiteGC,
          8, 0, 8, 5);
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
      width = attr.maxWidth;
    }
    if (attr.minWidth != -1 && attr.minWidth >= width ) {
      width = attr.minWidth;
    }  
    if (attr.maxHeight != -1 && attr.maxHeight <= height) {
      height = attr.maxHeight;
    }
    if (attr.minHeight != -1 && attr.minHeight >= height) {
      height = attr.minHeight;
    }

    printf("%d %d %d %d\n",attr.minHeight, attr.minWidth, height, width);

    if (!this->isMaximised) {
      pronResizeWindow(display, this->parent, width, height);
      pronMoveWindow(display, this->closeButton,
          width - this->parentAttributes.width, 0);
      pronMoveWindow(display, this->maximiseButton,
          width - this->parentAttributes.width, 0);
      pronMoveWindow(display, this->resizeButton,
          width - this->parentAttributes.width,
          height - this->parentAttributes.height);
      this->parentAttributes.width  = width;
      this->parentAttributes.height = height;
      this->attributes.width        = width  - 2 * borderSize;
      this->attributes.height       = height - titleBarSize - borderSize;
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
}


void GWindow::raise() {
  GWindow *oldRaisedWindow = raisedWindow;
  raisedWindow = this;
  pronRaiseWindow(display, this->parent);
  oldRaisedWindow->decorate();
}
