#ifndef _WINDOWS_TREE_H_ 
#define _WINDOWS_TREE_H_ 

#include <window.h>

class WindowsTree {

public:
	WindowsTree ();
	
	Window * getRoot();
	void setRoot(Window * newRoot);

	
	class Iterator {
      public:
      int i;

	};
	Iterator begin();
	Iterator end();


private:
	Window * root;

};


#endif