#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <proto/events.h>
#include <vector>
#include <drawable.h>

using namespace std;
using namespace pron;

class Screen;
class Client;
class drawable;

struct OtherClient {
  OtherClient(Client *client, unsigned int mask) {
    this->client = client;
    this->mask = mask;
  }

  Client *client;
  unsigned int mask;
};

class Window : public Drawable {
public: //XXX: bourrin

  // Window attributes
  int x, y;
  color_t bgColor;
  int eventMask;
  int dontPropagateMask;
  vector<OtherClient> otherClients;
  bool mapped;
  bool isResizable;
  int maxWidth;
  int maxHeight;
  int minWidth;
  int minHeight;

  Window *parent;
  Window *prevSibling, *nextSibling;
  Window *firstChild, *lastChild;

  Window(Screen *screen, int id, Client *creator, Window *parent, int x, int y, int width, int height);

  ~Window();

  void map();

  void unmap();

  void drawPoint(int x, int y);

  void drawLine(int x1, int y1, int x2, int y2);

  void drawRect(int x1, int y1, int width, int height);
  
  void fillRectangle(int x, int y, int width, int height);

  void putImage(PronImage *image, int x, int y);
  
  void drawCircle(int x, int y, int radius);

  void fillCircle(int x, int y, int radius);

  void clear();

  void clear(int x, int y, int width, int height);

  void clear(bool sendExposureEvent);

  void clear(int x, int y, int width, int height, bool sendExposureEvent);

  int getPixel(int x, int y);

  void setPixel(int x, int y, int pixel);

  PronWindowAttributes getAttributes();

  void setAttributes(PronWindowAttributes *newAttributes, unsigned int mask);

  void selectInput(Client *client, unsigned int mask);

  void raise();

  void exposeArea(int x, int y, int width, int height);

  void destroy();

  void move(int dx, int dy);

  void moveTo(int x, int y);

  void resize(int width, int height);

  /**
   * Delivers an event to this window.
   *
   * @param e The event to deliver
   * @param size The size of the event
   */
  void deliverEvent(PronEvent *e, unsigned int size);

  /**
   * Delivers an event to this window and its immediate parent.
   * Suitable for window events ().
   * 
   * @param e The event to deliver
   * @param size The size of the event
   */
  void deliverWindowEvent(PronEvent *e, unsigned int size);

  /**
   * Delivers a event that needs to propagate up the window tree.
   * Suitable for device events ().

   * @param e The event to deliver
   * @param size The size of the event
   */
  void deliverDeviceEvent(PronEvent *e, unsigned int size);

  /**
   * Returns true when window contains A(x,y) 
   */
  bool contains(int x, int y);

  /**
   * Getters and setters
   */

  int getX();
  void setX(int x);
  int getY();
  void setY(int y);

  /**
   * Sets a new parent to the Window.
   *
   * @param w The new parent Window
   */
  void reparent(Window *w);

  /**
   * Copies a Drawable area into the window.
   * @param dstX The top-left x-coordinate of the destination area
   * @param dstY The top-left y-coordinate of the destination area
   * @param d The source Drawable
   * @param srcX The top-left x-coordinate of the source area
   * @param srcY The top-left y-coordinate of the source area
   * @param width The width of the area
   * @param height The height of the area
   */
  void copyArea(int dstX, int dstY, Drawable *d, int srcX, int srcY, int width, int height);

  /**
   * Draws the given text.
   * @param x The x-coordinate of the origin of the first character
   * @param y The y-coordinate of the origin of the first character
   * @param text The text to draw
   * @param length The length of the text
   */
  void drawText(int x, int y, const char *text, int length);

private:
  
  bool overlaps(Window *w);
};

#endif
