#include <gwindows_manager.h>
#include <cstdio>

GWindowsManager::GWindowsManager(Window rootWindow) {
  this->rootWindow = rootWindow;
}

void GWindowsManager::addGWindow(GWindow *w) {
  this->windowsList.push_back(w);
}

GWindow* GWindowsManager::getGWindow(Window w) {
  for (size_t i = 0; i < this->windowsList.size(); ++i) {
    if (this->windowsList[i]->window            == w
          || this->windowsList[i]->parent       == w) {
      return this->windowsList[i];
    }
  }
  // TODO throw exception if not found
  return NULL;
}

PronWindowAttributes & GWindowsManager::getRootWindowAttributes() {
  return this->rootWindowAttributes;
}

void GWindowsManager::destroy(Window w) {
  for (size_t i = 0; i < this->windowsList.size(); ++i) {
    if (this->windowsList[i]->window == w || this->windowsList[i]->parent == w) {
      delete this->windowsList[i];
      this->windowsList[i] = this->windowsList[this->windowsList.size()-1];
      this->windowsList.pop_back();
    }
  }
}

bool GWindowsManager::overlaps(GWindow *gw) {
  bool res = false;
  for (size_t i = 0; i < this->windowsList.size(); ++i) {
    if (gw->overlaps(this->windowsList[i])) {
      res = true;
      break;
    }
  }
  return res;
}

void GWindowsManager::initWindowPosition(GWindow *gw) {
  gw->parentAttributes.x = 0;
  gw->parentAttributes.y = 0;
  if (!this->overlaps(gw)) {
    return;
  }
  for (size_t i = 0; i < this->windowsList.size(); ++i) {
    // Si ça rentre à droite
    if (this->windowsList[i]->parentAttributes.x + this->windowsList[i]->parentAttributes.width
          + gw->parentAttributes.width < rootWindowAttributes.width) {
      gw->parentAttributes.x = this->windowsList[i]->parentAttributes.x + this->windowsList[i]->parentAttributes.width;
      gw->parentAttributes.y = 0;
      if (!this->overlaps(gw)) {
        return;
      }
    }
    // Si ça rentre en dessous
    
    if (this->windowsList[i]->parentAttributes.y + this->windowsList[i]->parentAttributes.height
          + gw->parentAttributes.height < rootWindowAttributes.height) {
      gw->parentAttributes.x = 0;
      gw->parentAttributes.y = this->windowsList[i]->parentAttributes.y + this->windowsList[i]->parentAttributes.height;
      if (!this->overlaps(gw)) {
        return;
      }
      // En dessous à droite
      if (this->windowsList[i]->parentAttributes.x + this->windowsList[i]->parentAttributes.width
            + gw->parentAttributes.width < rootWindowAttributes.width) {
        gw->parentAttributes.x = this->windowsList[i]->parentAttributes.x + this->windowsList[i]->parentAttributes.width;
        gw->parentAttributes.y = this->windowsList[i]->parentAttributes.y + this->windowsList[i]->parentAttributes.height;
        if (!this->overlaps(gw)) {
          return;
        }
      }
    }
  }
  gw->parentAttributes.x = 0;
  gw->parentAttributes.y = 0;
}

bool GWindowsManager::empty() {
  return this->windowsList.empty();
}