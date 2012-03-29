#ifndef _WINDOWS_TREE_H_ 
#define _WINDOWS_TREE_H_ 

#include <window.h>
#include <queue>
#define END_OF_TREE NULL

using namespace std;

class WindowsTree {
  public:
    class Iterator {
      public:
      Iterator(Window * win);
      // pre incrementation 
      // TODO : implement this
      Iterator operator++(); 
      // post incrementation
      Iterator operator++(int junk);

      // Returns *(this->currentWindow)
      Window & operator*();

      // Returns *(this->currentWindow)
      Window * operator->();

      // Compares currentWindows attributes
      bool operator==(Iterator it);

      // Compares currentWindows attributes
      bool operator!=(Iterator it);

      private:
      Window * localRoot; // The window from where we start discovering the tree (by default : the root of the windowsTree)
      Window * currentWindow; // The window that will be return by a call to *iterator
      queue<Window *> winQueue; // The queue of the windows not yet covered (actually we only save the first child of their parent)
    };

    WindowsTree ();

    Window * getRoot();
    void setRoot(Window * newRoot);

    Iterator begin();
    Iterator begin(Window * root);
    Iterator end();

  private:
    Window * root; // Root of the windowsTree

};


#endif