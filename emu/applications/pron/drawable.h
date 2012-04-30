#ifndef _DRAWABLE_H_
#define _DRAWABLE_H_

#include <proto/bits/image.h>

using namespace std;
using namespace pron;

class Window;
//class PixMap;
class Screen;
class Client;

/**
 * Drawable type
 */
enum DrawableType {
  D_WINDOW = 0x0000, 
  D_PIXMAP = 0x0001,
};

class Drawable {

protected: 
  /**
  * Constructors
  */
  Drawable(int type, Screen *screen, int id, Client *creator, int width, int height);
  ~Drawable(); 

  /**
   * Reduces the given rectangle so that it fits into the drawable.
   * @param x The top-left x-coordinate of the rectangle, relative to this drawable
   * @param y The top-left y-coordinate of the rectangle, relative to this drawable
   * @param width The width of the rectangle
   * @param height The height of the rectangle
   */
  void reduce(int &x, int &y, int &width, int &height);
public: 

  /**
   * Getters and setters
   */
  int getType();
  Screen* getScreen();
  unsigned int getId() const;
  Client* getCreator();
  int getWidth();
  void setWidth(int width);
  int getHeight();
  void setHeight(int height);

  /**
   * Graphic methods
   */
  virtual void* pixelAddr(int x, int y) = 0;
  virtual bool isValid(int x, int y) = 0;

  void drawHorizLine(int x, int y, int width);
  void drawVertLine(int x, int y, int height);

  void drawPoint(int x, int y);
  void drawLine(int x1, int y1, int x2, int y2);
  void drawRect(int x1, int y1, int width, int height);
  void drawCircle(int x, int y, int radius);
  void fillRectangle(int x, int y, int width, int height);
  void fillCircle(int x, int y, int radius);
  void putImage(PronImage *image, int x, int y);
  
  int getPixel(int x, int y);
  void setPixel(int x, int y, int pixel);

private:

  // Which kind of drawable is
  unsigned int type;
  // Drawable attributes
  Screen *screen;
  unsigned int id;
  Client *creator;
  int width, height;

};

#endif
