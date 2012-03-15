#include <gwindows_manager.h>

GWindow::GWindow (Window w, Window parent) {
  this->window = w;
  this->parent = parent;
  this->x      = 0;
  this->y      = 0;
  this->width  = 0;
  this->height = 0;
}

bool GWindow::hasDecoration() {
  return (parent == 0);
}
