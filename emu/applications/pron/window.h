#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <pronlib_structs.h>

class Screen;

class Window {
    public: //XXX: bourrin
	Screen *screen;

	int id;

	// Window attributes
	int x, y;
	int width, height;

	Window *parent;
	Window *prevSibling, *nextSibling;
	Window *firstChild, *lastChild;

	Window(Screen *screen, int id, Window *parent, int x, int y, int width, int height);

	void drawPoint(int x, int y);

	void drawLine(int x1, int y1, int x2, int y2);

	void fillRectangle(int x, int y, int width, int height);

	void clear();

	void flush();

	PronWindowAttributes * getAttributes();

	void setAttributes(PronWindowAttributes * newAttributes, int mask);
};


#endif
