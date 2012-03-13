#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <vector>
#include <clip_zone.h>

extern "C" {
#include <vesa_types.h>
}

using namespace std;

class Window;

struct GC {
  color_t fg;
};

class Screen {

private:
  Screen(int width, int height, int bitsPerPixel);
  static Screen *instance;
  int mouseX; // Mouse state
  int mouseY;
  bool mouseB1;
  bool mouseB2;
  bool mouseB3;
  bool mouseB4;
  bool mouseB5;
  bool mouseB6;
  Window * mouseWin; // First window containing the mouse pointer

  void drawHorizLine(int x, int y, int width);
  void drawVertLine(int x, int y, int height);

public: // XXX: bourrin
  int width, height;
  int bitsPerPixel;
  int bytesPerPixel;
  int vesa_fd;
  char *videoBuffer;
  vector<Window*> windows; // XXX: ABR ? Rouge/noir ? B-Arbre ?
  Window *root;
  Window *clipWin;
  ClipZone *clipZone;
  GC gc;

  static Screen* getInstance(int width, int height, int bitsPerPixel);
  static Screen* getInstance();
  
  void setClipWindow(Window *w);
  bool prepareDrawing(Window *w);

  void drawPoint(int x, int y);
  void drawLine(int x1, int y1, int x2, int y2);
  void drawRect(int x, int y, int width, int height) ;
  void drawCircle(int x, int y, int r);

  void fillCircle(int n_cx, int n_cy, int radius);
  void fillRectangle(int x, int y, int width, int height);

  Window* getWindow(unsigned int id);
  void addWindow(Window *w);

  void setMouseX(int mouseX);
  int getMouseX();
  void setMouseY(int mouseY);
  int getMouseY();
  void setMouseB1(bool mouseB1);
  bool getMouseB1();
  void setMouseB2(bool mouseB2);
  bool getMouseB2();
  void setMouseB3(bool mouseB3);
  bool getMouseB3();
  void setMouseB4(bool mouseB4);
  bool getMouseB4();
  void setMouseB5(bool mouseB5);
  bool getMouseB5();
  void setMouseB6(bool mouseB6);
  bool getMouseB6();

  Window* getMouseWin();
  void setMouseWin(Window *mouseWin);
  Window* getRoot();
  void setRoot(Window *root);

  void traceWindows();
};

#endif
