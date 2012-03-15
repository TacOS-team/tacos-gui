#include <gwindows_manager.h>

void GWindowsManager::addGWindow(GWindow *w) {
  this->windowsList.push_back(w);
}

GWindow* GWindowsManager::getGWindow(Window w) {
  for (size_t i = 0; i < this->windowsList.size(); ++i) {
    if (this->windowsList[i]->window == w || this->windowsList[i]->parent == w) {
      return this->windowsList[i];
    }
  }
  // TODO throw exception if not found
  return NULL;
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

bool GWindowsManager::empty() {
  return this->windowsList.empty();
}