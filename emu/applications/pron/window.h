#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <pronlib_structs.h>
#include <vector>

using namespace std;

class Screen;

struct OtherClient {
  OtherClient(int id, unsigned int mask) {
    this->id = id;
    this->mask = mask;
  }

  int id;
  unsigned int mask;
};

class Window {
public: //XXX: bourrin
  Screen *screen;

  int id;

  // Window attributes
  int x, y;
  int width, height;
  int eventMask;
  vector<OtherClient> otherClients;

  Window *parent;
  Window *prevSibling, *nextSibling;
  Window *firstChild, *lastChild;

  Window(Screen *screen, int id, Window *parent, int x, int y, int width, int height);

  short getCreator();

  void drawPoint(int x, int y);

  void drawLine(int x1, int y1, int x2, int y2);

  void drawRect(int x1, int y1, int w, int h) ;
	
  void fillRectangle(int x, int y, int width, int height);

  void clear();

  void flush();

  PronWindowAttributes getAttributes();

  void setAttributes(PronWindowAttributes *newAttributes, unsigned int mask);

  void selectInput(int client, unsigned int mask);
};

#endif
