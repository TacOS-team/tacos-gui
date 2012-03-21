#include <windowsTree.h>

using namespace std;

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
	WindowsTree::Iterator it;
	return it;
}


WindowsTree::Iterator WindowsTree::end() {
	WindowsTree::Iterator it;
	return it;
}	


