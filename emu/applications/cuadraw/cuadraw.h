#ifndef __CUADRAW_H__
#define __CUADRAW_H__
#include <sombrero.h>

#define TEMP_NAM_PREFIX "cuad"
#define TEMP_NAM_FOLDER "/tmp"

class CoolCanvas;
class ColorPickerWindow;

class Cuadraw : public has_slots<> {
 private:
  FILE *fileIn; /**< The input file */
  FILE *fileOut; /**< The output file */
  sombrero::Window *w; /**< The main window */
  CoolCanvas *c; /**< The canvas of the where to draw */
  sombrero::Grid *g; /**< The sombrero grid */
  sombrero::Button *bp; /**< Pixel brush button */
  sombrero::Button *bl; /**< Line brush button */
  sombrero::Button *bc; /**< Circle brush button */
  sombrero::Button *bfc; /**< Fill circle brush button */
  sombrero::Button *br; /**< Rect brush button */
  sombrero::Button *bfr; /**< Fill Rect brush button */
  sombrero::Button *bcp; /**< Color picker button */
  sombrero::ColorPicker *cp; /**< Color picker */
  ColorPickerWindow *wcp; /**< Color picker window */
  bool mouseDown; /**< Mouse down in canvas */
  int downX; /**< Last mouse down x */
  int downY; /**< Last mouse down y */
  /**
   * Initialize cuadraw
   * @param argc
   * @param argv
   */
  void init(int argc, char **argv);
  /**
   * Initialize sombrero stuff
   */
  void initSombrero();
 protected:
 public:
  void clearSignals();
  /**
   * Cuadraw constructor
   * @param argc
   * @param argv
   */
  Cuadraw(int argc, char **argv);
  /**
   * Cuadraw destructor
   */
  ~Cuadraw();
  /**
   * Lanches the cuadraw application
   */
  void run();
  /**
   * Do the pixel brush action
   * Pixel button click handler
   */
  void doPixelClicked();
  /**
   * Pixel Brush mouse moved
   * @param xMove x move
   * @param yMove y move
   * @param x x coordinate
   * @param y y coordinate
   */
  void pixelMouseMoved(int x, int y, int aX, int aY);
  /**
   * Pixel Brush mouse clicked
   * @param b MouseBotton button state
   */
  void pixelMouseClicked(sombrero::MouseButton, int x, int y);
  /**
   * Pixel Brush mouse released
   * @param b MouseBotton button state
   */
  void pixelMouseReleased(sombrero::MouseButton, int x, int y);
  /**
   * Changes the mode of drawing to Line Brush
   * Circle button click handler
   */
  void doLineClicked();
  /**
   * Line Brush mouse moved
   * @param xMove x move
   * @param yMove y move
   * @param x x coordinate
   * @param y y coordinate
   */
  void lineMouseMoved(int x, int y, int aX, int aY);
  /**
   * Line Brush mouse clicked
   * @param b MouseBotton button state
   */
  void lineMouseClicked(sombrero::MouseButton, int x, int y);
  /**
   * Line Brush mouse released
   * @param b MouseBotton button state
   */
  void lineMouseReleased(sombrero::MouseButton, int x, int y);
  /**
   * Changes the mode of drawing to Circle Brush
   * Circle button click handler
   */
  void doCircleClicked();
  /**
   * Circle Brush mouse moved
   * @param xMove x move
   * @param yMove y move
   * @param x x coordinate
   * @param y y coordinate
   */
  void circleMouseMoved(int x, int y, int aX, int aY);
  /**
   * Circle Brush mouse clicked
   * @param b MouseBotton button state
   */
  void circleMouseClicked(sombrero::MouseButton, int x, int y);
  /**
   * Circle Brush mouse released
   * @param b MouseBotton button state
   */
  void circleMouseReleased(sombrero::MouseButton, int x, int y);
  /**
   * Changes the mode of drawing to Fill Circle Brush
   * Circle button click handler
   */
  void doFillCircleClicked();
  /**
   * Fill Circle Brush mouse moved
   * @param xMove x move
   * @param yMove y move
   * @param x x coordinate
   * @param y y coordinate
   */
  void fillCircleMouseMoved(int x, int y, int aX, int aY);
  /**
   * Fill Circle Brush mouse clicked
   * @param b MouseBotton button state
   */
  void fillCircleMouseClicked(sombrero::MouseButton, int x, int y);
  /**
   * Fill Circle Brush mouse released
   * @param b MouseBotton button state
   */
  void fillCircleMouseReleased(sombrero::MouseButton, int x, int y);
  /**
   * Changes the mode of drawing to Rect Brush
   * Rect button click handler
   */
  void doRectClicked();
  /**
   * Rect Brush mouse moved
   * @param xMove x move
   * @param yMove y move
   * @param x x coordinate
   * @param y y coordinate
   */
  void rectMouseMoved(int x, int y, int aX, int aY);
  /**
   * Rect Brush mouse clicked
   * @param b MouseBotton button state
   */
  void rectMouseClicked(sombrero::MouseButton, int x, int y);
  /**
   * Rect Brush mouse released
   * @param b MouseBotton button state
   */
  void rectMouseReleased(sombrero::MouseButton, int x, int y);
  /**
   * Changes the mode of drawing to Rect Brush
   * Fill Rect button click handler
   */
  void doFillRectClicked();
  /**
   * Fill Rect Brush mouse moved
   * @param xMove x move
   * @param yMove y move
   * @param x x coordinate
   * @param y y coordinate
   */
  void fillRectMouseMoved(int x, int y, int aX, int aY);
  /**
   * Rect Brush mouse clicked
   * @param b MouseBotton button state
   */
  void fillRectMouseClicked(sombrero::MouseButton, int x, int y);
  /**
   * Fill Rect Brush mouse released
   * @param b MouseBotton button state
   */
  void fillRectMouseReleased(sombrero::MouseButton, int x, int y);
  /**
   * Slot for the signal color changed of colorpicker
   * @param c Color to use
   */
  void doColorChanged(Color c);
  /**
   * Opens a color picker window to select more colors
   */
  void doColorPickerWindow();
};
#endif//__CUADRAW_H__
