#ifndef _DRAWABLE_H_
#define _DRAWABLE_H_

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
  void drawPoint(int x, int y);
  void drawLine(int x1, int y1, int x2, int y2);
  void drawRect(int x1, int y1, int width, int height);
  void fillRectangle(int x, int y, int width, int height);
  void putImage(PronImage *image, int x, int y);
  void drawCircle(int x, int y, int radius);
  void fillCircle(int x, int y, int radius);
  void clear();
  void clear(int x, int y, int width, int height);

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