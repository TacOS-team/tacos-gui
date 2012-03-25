#ifndef _DRAWABLE_H_
#define _DRAWABLE_H_
#include <pronlib_structs.h>

using namespace std;

class Window;
//class PixMap;
class Screen;
class Client;

class Drawable {

protected: 
  /**
  * Constructors
  */
  Drawable(int type, Screen *screen, int id, Client *creator, int width, int height);
  ~Drawable(); 

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