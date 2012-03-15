#include <gwindows_manager.h>
#include <cstdio>


GWindow::GWindow (Window w, Window parent) {
  this->window = w;
  this->parent = parent;
}

bool GWindow::hasDecoration() {
  return (parent == 0);
}

bool GWindow::overlaps(GWindow *gw) {
  printf("overlaps this (x, y, width, height) : %d, %d, %d, %d\n",
    this->parentAttributes.x, this->parentAttributes.y,
    this->parentAttributes.width, this->parentAttributes.height);
  printf("overlaps gw (x, y, width, height) : %d, %d, %d, %d\n",
    gw->parentAttributes.x, gw->parentAttributes.y,
    gw->parentAttributes.width, gw->parentAttributes.height);
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
  printf("overlaps : %s\n", res ? "true" : "false");
  return res;
}
