#ifndef __SCREEN_H__
#define __SCREEN_H__

/**
 * @file screen.h
 * Screen class definition.
 */

#include <font.h>
#include <vector>
#include <clip_zone.h>
#include <gc.h>
#include <windowsTree.h>
#include <vesa_types.h>

using namespace std;

class Window;

/**
 * Screen.
 * @todo XXX public members bourrin
 */
class Screen {
 private:
  static Screen *instance; /**< Unique Screen instance (singleton) */
  vector<Font*> fonts; /**< Fonts loaded on the server */
  Window *clipWin; /**< Window for which the clipping zone is set */
  ClipZone *clipZone; /**< Rectangles where we are allowed to draw */
  Window *mouseWin; /**< Window containing the mouse pointer */
  Window *focusWin; /**< Window having the focus */
  GC defaultGC; /**< Default graphics context for this screen */
  char *videoBuffer; /**< Video memory */
  GC *gc; /**< Current graphics context */
  int vesa_fd; /**< File descriptor used to communicate with the vesa driver */
  vector<Drawable*> drawables; /**< Drawables belonging to this screen @todo XXX ABR ? Rouge/noir ? B-Arbre ? */
  
  /**
   * Constructor.
   * @param width The width of the screen
   * @param height The height of the screen
   * @param bitsPerPixel The number of bits per pixel (8, 16, 24, 32)
   */
  Screen(int width, int height, int bitsPerPixel);

 public:
  int width; /**< Width of the screen */
  int height; /**< Height of the screen */
  int bitsPerPixel; /**< Bits per pixel (8, 16, 24, 32) */
  int bytesPerPixel; /**< Bytes per pixel (1, 2, 3, 4) */
  WindowsTree *tree; /**< Tree of windows belonging to this screen */

  /**
   * Returns the unique Screen instance (singleton). Creates it if necessary.
   * @param width The width of the screen
   * @param height The height of the screen
   * @param bitsPerPixel The number of bits per pixel (8, 16, 24, 32)
   * @return the unique Screen instance (singleton)
   */
  static Screen* getInstance(int width, int height, int bitsPerPixel);
  
  /**
   * Returns the unique Screen instance (singleton).
   * @return the unique Screen instance (singleton)
   */
  static Screen* getInstance();

  /**
   * Gets the address of the pixel (x, y) in memory.
   * @param x The x-coordinate of the pixel
   * @param y The y-coordinate of the pixel
   */ 
  void* pixelAddr(int x, int y);
  
  /**
   * Returns true if we can draw at position (x, y).
   * @param x The x-coordinate of the point to check
   * @param y The y-coordinate of the point to check
   * @return true if we can draw at position (x, y)
   */
  bool isValid(int x, int y);
  
  /**
   * Prepares the screen for drawing in the given drawable,
   * with the given graphics context.
   * @param d The drawable where to draw
   * @param gc The graphics context to draw with
   * @return success
   */
  bool prepareDrawing(Drawable *d, GC *gc = NULL);

  /**
   * Gets the queried drawable from the drawable list.
   * @param id The id of the queried drawable
   * @param drawableType The type of the queried drawable (window or pixmap)
   * @return the queried drawable (NULL if not found)
   */
  Drawable* getDrawable(unsigned int id, int drawableType);

  /**
   * Gets the queried drawable from the drawable list.
   * @param id The id of the queried drawable
   * @return the queried drawable (NULL if not found)
   */
  Drawable* getDrawable(unsigned int id);

  /**
   * Adds the given drawable to the drawable list.
   * @param d The drawable to add
   */
  void addDrawable(Drawable *d);

  /**
   * Removes the given drawable from the drawable list.
   * @param d The drawable to remove
   */
  void removeDrawable(Drawable *d);

  /**
   * Returns the window for which the clipping is currently set.
   * @return the window for which the clipping is currently set
   */
  Window* getClipWin();

  /**
   * Sets the clipping for the given window.
   * @param w The window for which to set the clipping
   */
  void setClipWin(Window *w);

  /**
   * Returns the window the mouse is currently in.
   * @return the window the mouse is currently in
   */
  Window* getMouseWin();

  /**
   * Sets the window the mouse is currently in.
   * @param mouseWin The window the mouse is currently in
   */
  void setMouseWin(Window *mouseWin);

  /**
   * Returns the window that currently has the focus.
   * @return the window that currently has the focus.
   */
  Window* getFocusWin();

  /**
   * Sets the window that currently has the focus.
   * @param focusWin The window that currently has the focus
   */
  void setFocusWin(Window *focusWin);

  /**
   * Returns the root of the windows tree.
   * @return the root of the windows tree
   */
  Window* getRoot();

  /**
   * Destroys the given window.
   * @param w The window to destroy
   */
  void destroy(Window * w);

  /**
   * Sets a new parent to the given window.
   * @param child The window to reparent
   * @param newParent The new parent window
   */
  void reparent(Window *child, Window *newParent);

  /**
   * Returns the current graphics context.
   * @return the current graphics context
   */
  GC* getGC();

  /**
   * Sets the current graphics context.
   * @param gc The new graphics context to use
   */
  void setGC(GC *gc);

  /**
   * Gets the requested font from the font list.
   * @param id The font id
   * @return The requested font
   */
  Font* getFont(int id);

  /**
   * Prints the clipping zone (for debugging purposes).
   */
  void printClipZone();

  /**
   * Prints the windows tree (for debugging purposes).
   */
  void traceWindows();
};

#endif
