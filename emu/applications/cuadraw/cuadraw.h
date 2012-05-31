#ifndef __CUADRAW_H__
#define __CUADRAW_H__
#include <sombrero.h>

#define TEMP_NAM_PREFIX "cuad"
#define TEMP_NAM_FOLDER "/tmp"

class CoolCanvas;

class Cuadraw : public has_slots<> {
 private:
  FILE *fileIn; /**< The input file */
  FILE *fileOut; /**< The output file */
  sombrero::Window *w; /**< The main window */
  CoolCanvas *c; /**< The canvas of the where to draw */
  sombrero::Grid *g; /**< The sombrero grid */
  sombrero::Button *bp; /**< Pixel brush button */
  sombrero::Button *bc; /**< Circle brush button */
  bool mouseDown; /**< Mouse down in canvas */
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
   void pixelMouseClicked(sombrero::MouseButton);
   /**
    * Pixel Brush mouse released
    * @param b MouseBotton button state
    */
   void pixelMouseReleased(sombrero::MouseButton);
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
   void circleMouseClicked(sombrero::MouseButton);
   /**
    * Circle Brush mouse released
    * @param b MouseBotton button state
    */
   void circleMouseReleased(sombrero::MouseButton);
};
#endif//__CUADRAW_H__
