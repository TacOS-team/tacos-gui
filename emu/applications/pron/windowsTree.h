#ifndef _WINDOWS_TREE_H_ 
#define _WINDOWS_TREE_H_ 

#include <window.h>

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
      Window * currentWindow;
    };

    WindowsTree ();

    Window * getRoot();
    void setRoot(Window * newRoot);

    Iterator begin();
    Iterator end();

  private:
    Window * root;

};


#endif