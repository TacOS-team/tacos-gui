#include <windowsTree.h>

/* WindowsTree methods */

WindowsTree::WindowsTree() {
  this->root = NULL;
}

Window * WindowsTree::getRoot() {
  return this->root;
}

void WindowsTree::setRoot(Window * newRoot) {
  this->root = newRoot;
}


WindowsTree::Iterator WindowsTree::begin() {
  WindowsTree::Iterator it(this->root);
  return it;
}

WindowsTree::Iterator WindowsTree::end() {
  
  Window * win = this->root;
  while (win->lastChild != NULL) {
    win = win->lastChild;
  }
  WindowsTree::Iterator it(win);
  

  return it;
}   




/* WindowsTree::Iterator methods */

WindowsTree::Iterator::Iterator(Window * win) {
  this->currentWindow = win;
}

WindowsTree::Iterator WindowsTree::Iterator::operator++() {
  return *this;
}

WindowsTree::Iterator WindowsTree::Iterator::operator++(int junk) {
  // We go to the right of the tree as long as we can
  if (this->currentWindow->nextSibling != NULL) {
    this->currentWindow = this->currentWindow->nextSibling;
  } else { // If we can't go right, then we have to go to the next level of the tree
    if (this->currentWindow->parent == NULL) { // We are the root window, just go to first child
      this->currentWindow = this->currentWindow->firstChild;
    } else {
      // We return to the first child of the parent of currentWindow
      this->currentWindow = this->currentWindow->parent->firstChild;
      // and then we continue to the first window we encounter at the next level of the tree
      while (this->currentWindow->firstChild != NULL && this->currentWindow->nextSibling != NULL) {
        this->currentWindow = this->currentWindow->nextSibling;
      }
      // We're still not sure the current window actually has a child...
      if (this->currentWindow->firstChild != NULL) {
        this->currentWindow = this->currentWindow->firstChild;
      }
    }
  }
  return *this;
}


Window & WindowsTree::Iterator::operator*() {
  return *(this->currentWindow);
}

Window * WindowsTree::Iterator::operator->() {
  return (this->currentWindow);
}

bool WindowsTree::Iterator::operator==(Iterator it) {
  return (it.currentWindow == this->currentWindow);
}

bool WindowsTree::Iterator::operator!=(Iterator it) {
  return (it.currentWindow != this->currentWindow);
}