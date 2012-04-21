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

  void reduce(int &x, int &y, int &width, int &height);
public: 

  /**
   * Getters and setters
   */
  int getType();
  Screen* getScreen();
  unsigned int getId();
  Client* getCreator();
  int getWidth();
  void setWidth(int width);
  int getHeight();
  void setHeight(int height);

  /**
   * Graphic methods
   */
  virtual void drawPoint(int x, int y) = 0;
  virtual void drawLine(int x1, int y1, int x2, int y2) = 0;
  virtual void drawRect(int x1, int y1, int width, int height) = 0;
  virtual void fillRectangle(int x, int y, int width, int height) = 0;
  virtual void putImage(PronImage *image, int x, int y) = 0;
  virtual void drawCircle(int x, int y, int radius) = 0;
  virtual void fillCircle(int x, int y, int radius) = 0;
  virtual void clear() = 0;
  virtual void clear(int x, int y, int width, int height) = 0;
  virtual int getPixel(int x, int y) = 0;
  virtual void setPixel(int x, int y, int pixel) = 0;

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
