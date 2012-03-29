#include <windowsTree.h>
#include <cstdio>

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

WindowsTree::Iterator WindowsTree::begin(Window * root) {
  WindowsTree::Iterator it(root);
  return it;
}

WindowsTree::Iterator WindowsTree::end() {
  Window * win = END_OF_TREE;
  WindowsTree::Iterator it(win);
  return it;
}   




/* WindowsTree::Iterator methods */

WindowsTree::Iterator::Iterator(Window * localRoot) {
  this->currentWindow = this->localRoot = localRoot;
  if (this->localRoot != END_OF_TREE && this->localRoot->firstChild != NULL) {
    this->winQueue.push(this->localRoot->firstChild); 
  }
}

WindowsTree::Iterator WindowsTree::Iterator::operator++() {
  return *this;
}

WindowsTree::Iterator WindowsTree::Iterator::operator++(int junk) {
  /* 
  Breadth First Search algorithm
  We only push the first child of a window because we can access all the other childs by using nextSibling.
  */
  // Go right while you can
   if (this->currentWindow != this->localRoot && this->currentWindow->nextSibling != NULL) { 
    this->currentWindow = this->currentWindow->nextSibling;
  } else {
    if (!this->winQueue.empty()) {
      this->currentWindow = this->winQueue.front();
      this->winQueue.pop();
    } else { 
      // here we're finished
      this->currentWindow = END_OF_TREE;
    }
  }
  // Pushing the first child
  if (this->currentWindow != END_OF_TREE && this->currentWindow->firstChild != NULL) {
      this->winQueue.push(this->currentWindow->firstChild);
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