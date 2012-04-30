#ifndef _WINDOWS_TREE_H_ 
#define _WINDOWS_TREE_H_ 

#include <window.h>
#include <queue>
#define END_OF_TREE NULL

using namespace std;

class WindowsTree {
  public:
    class IteratorBFS {
      public:
      IteratorBFS(Window * win);
      // pre incrementation 
      IteratorBFS operator++(); 
      // post incrementation
      IteratorBFS operator++(int junk);

      // Returns *(this->currentWindow)
      Window & operator*();

      // Returns *(this->currentWindow)
      Window * operator->();

      // Compares currentWindows attributes
      bool operator==(IteratorBFS it);

      // Compares currentWindows attributes
      bool operator!=(IteratorBFS it);

      private:
      Window * localRoot; // The window from where we start discovering the tree (by default : the root of the windowsTree)
      Window * currentWindow; // The window that will be return by a call to *Iterator
      queue<Window *> winQueue; // The queue of the windows not yet covered (actually we only save the first child of their parent)
    };

    class IteratorDFS {
      public:
      IteratorDFS(Window * win);
      // pre incrementation 
      IteratorDFS operator++(); 
      // post incrementation
      IteratorDFS operator++(int junk);

      // Returns *(this->currentWindow)
      Window & operator*();

      // Returns *(this->currentWindow)
      Window * operator->();

      // Compares currentWindows attributes
      bool operator==(IteratorDFS it);

      // Compares currentWindows attributes
      bool operator!=(IteratorDFS it);

      private:
      Window * localRoot; // The window from where we start discovering the tree (by default : the root of the windowsTree)
      Window * currentWindow; // The window that will be return by a call to *Iterator
    };


    WindowsTree ();

    Window * getRoot();
    void setRoot(Window * newRoot);

    IteratorBFS beginBFS();
    IteratorBFS beginBFS(Window * root);
    IteratorBFS endBFS();

    IteratorDFS beginDFS();
    IteratorDFS beginDFS(Window * root);
    IteratorDFS endDFS();

  private:
    Window * root; // Root of the windowsTree

};


#endif