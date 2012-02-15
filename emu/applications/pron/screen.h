#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <vector>

using namespace std;

class Window;

class Screen {
public: // XXX: bourrin
	int width, height;
	int bitsPerPixel;
	int bytesPerPixel;
	int vesa_fd;
	char *videoBuffer;
	vector<Window*> windows; // XXX: ABR ? Rouge/noir ? B-Arbre ?
	Window *root;

	Screen(int width, int height, int bitsPerPixel);

	void drawPoint(int x, int y/*, color_t c*/);
	
	void drawLine(int x1, int y1, int x2, int y2/*, color_t color*/);
	void drawRect(int x, int y, int width, int height/*, color_t color*/) ;

	void fillRectangle(int x, int y, int width, int height/*, color_t color*/);

	Window* getWindow(int id);

	void addWindow(Window *w);

	void flush();
};

#endif
