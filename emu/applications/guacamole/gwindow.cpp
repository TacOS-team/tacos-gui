#include <gwindows_manager.h>
#include <cstdio>


GWindow::GWindow (Window w, Window parent) {
  this->window       = w;
  this->parent       = parent;
  this->closeButton  = 0;
  this->resizeButton = 0;
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
