#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <pronlib_enums.h>
#include <pronlib_structs.h>
#include <pronlib_events.h>
#include <vector>

using namespace std;

class Screen;
class Client;

struct OtherClient {
  OtherClient(Client *client, unsigned int mask) {
    this->client = client;
    this->mask = mask;
  }

  Client *client;
  unsigned int mask;
};

class Window {
public: //XXX: bourrin
  Screen *screen;

  int id;
  Client *creator;

  // Window attributes
  int x, y;
  int width, height;
  int eventMask;
  int dontPropagateMask;
  vector<OtherClient> otherClients;

  Window *parent;
  Window *prevSibling, *nextSibling;
  Window *firstChild, *lastChild;

  Window(Screen *screen, int id, Client *creator, Window *parent, int x, int y, int width, int height);

  //short getCreator();

  Client* getCreator();

  void drawPoint(int x, int y);

  void drawLine(int x1, int y1, int x2, int y2);

  void drawRect(int x1, int y1, int width, int height) ;
  
  void fillRectangle(int x, int y, int width, int height);
  
  void drawCircle(int x, int y, int radius);

  void fillCircle(int x, int y, int radius);

  void clear();

  void flush();

  PronWindowAttributes getAttributes();

  void setAttributes(PronWindowAttributes *newAttributes, unsigned int mask);

  void selectInput(Client *client, unsigned int mask);

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
};

#endif
