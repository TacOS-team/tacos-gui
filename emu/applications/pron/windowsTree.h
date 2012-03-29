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
        Iterator operator++(); 
      // post incrementation
      Iterator operator++(int junk);

      Window & operator*();

      Window * operator->();

      bool operator==(Iterator it);

      bool operator!=(Iterator it);

      private:
      Window * root; // The window from where we start discovering the tree
      Window * currentWindow; // The window that will be return by *iterator
      queue<Window *> winQueue; // The queue of the windows not yet covered (actually we only save the first child of their parent)
    };

    WindowsTree ();

    Window * getRoot();
    void setRoot(Window * newRoot);

    Iterator begin();
    Iterator begin(Window * root);
    Iterator end();

  private:
    Window * root;

};


#endif