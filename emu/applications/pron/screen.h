#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <font.h>
#include <vector>
#include <clip_zone.h>
#include <gc.h>
#include <windowsTree.h>

extern "C" {
#include <vesa_types.h>
}

using namespace std;

class Window;

class Screen {

private:
  Screen(int width, int height, int bitsPerPixel);
  static Screen *instance;
  vector<Font*> fonts;
  Window *clipWin; /**< Window for which the clipping zone is set */
  ClipZone *clipZone; /**< Rectangles where we are allowed to draw */
  Window *mouseWin; /**< Window containing the mouse pointer */
  Window *focusWin; /**< Window having the focus */

public: // XXX: bourrin
  int width, height;
  int bitsPerPixel;
  int bytesPerPixel;
  int vesa_fd;
  vector<Drawable*> drawables; // XXX: ABR ? Rouge/noir ? B-Arbre ?
  WindowsTree *tree;
  GC defaultGC;
  GC *gc;
  char *videoBuffer; /**< Video memory */

  static Screen* getInstance(int width, int height, int bitsPerPixel);
  static Screen* getInstance();

  void* pixelAddr(int x, int y);
  
  bool prepareDrawing(Drawable *d, GC *gc = NULL);

  Drawable* getDrawable(unsigned int id, int drawableType);
  Drawable* getDrawable(unsigned int id);
  void addDrawable(Drawable *d);

  Window* getClipWin();
  void setClipWin(Window *w);

  Window* getMouseWin();
  void setMouseWin(Window *mouseWin);

  /**
   * Returns the window that has the focus.
   * @return the window that has the focus.
   */
  Window* getFocusWin();

  /**
   * Sets the window that has the focus.
   */
  void setFocusWin(Window *focusWin);

  Window* getRoot();
  void setRoot(Window *root);

  void printClipZone();

  void traceWindows();

  void destroy(Window * w);

  void reparent (Window * child, Window * newParent);

  GC* getGC();
  void setGC(GC *gc);

  /**
   * Gets the requested font.
   * @param id The font id
   * @return The requested font
   */
  Font* getFont(int id);
  
  bool isValid(int x, int y);
};

#endif
