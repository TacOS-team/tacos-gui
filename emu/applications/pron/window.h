#ifndef __WINDOW_H__
#define __WINDOW_H__

/**
 * @file window.h
 * Window class definition.
 */

#include <proto/events.h>
#include <vector>
#include <drawable.h>

using namespace std;
using namespace pron;

class Screen;
class Client;
class drawable;

/**
 * Client who is not the creator but has selected events on the window.
 */
struct OtherClient {
  /**
   * Constructor.
   * @param client The client
   * @param mask The mask that specifies the selected events
   */
  OtherClient(Client *client, unsigned int mask) {
    this->client = client;
    this->mask = mask;
  }

  Client *client; /**< Client */
  unsigned int mask; /**< Mask that specifies the selected events */
};

/**
 * Window (drawable displayed on the screen).
 * @todo XXX public members bourrin
 */
class Window : public Drawable {
 public:
  int eventMask; /**< Events selected by the creator of the window */
  int dontPropagateMask; /**< Events that the window must not propagate */
  
  Window *parent; /**< Parent window */
  Window *prevSibling; /**< Previous sibling (linked list) */
  Window *nextSibling; /**< Next sibling (linked list) */
  Window *firstChild; /**< First child (linked list) */
  Window *lastChild; /**< Last child (linked list) */

  /**
   * Constructor.
   * @param screen The screen the window belongs to
   * @param id The id of the window
   * @param creator The client who has created the window
   * @param parent The parent window
   * @param x The x-coordinate of the top-left corner of the window
   * @param y The y-coordinate of the top-left corner of the window
   * @param width The width of the window
   * @param height The height of the window
   */
  Window(Screen *screen, int id, Client *creator, Window *parent, int x, int y, int width, int height);

  /**
   * Destructor.
   */
  ~Window();
  
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
   * Operator ==.
   * @param w The window to compare with
   * @return true if the windows are the same
   */
  bool operator==(const Window &w) const;
  
  /**
   * Operator !=.
   * @param w The window to compare with
   * @return true if the windows are the different
   */
  bool operator!=(const Window &w) const;

  /**
   * Maps this window (shows it on the screen).
   */
  void map();

  /**
   * Unmaps this (hides it from the screen).
   */
  void unmap();

  /**
   * Clears this window.
   * @param sendExposureEvent Whether to send an exposure event or not
   */
  void clear(bool sendExposureEvent = true);

  /**
   * Clears an area of this window.
   * @param x The x-coordinate of the top-left corner of the area to clear
   * @param y The y-coordinate of the top-left corner of the area to clear
   * @param width The width of the area to clear
   * @param height The height of the area to clear
   * @param sendExposureEvent Whether to send an exposure event or not
   */
  void clear(int x, int y, int width, int height, bool sendExposureEvent = true);

  /**
   * Returns the attributes of this window.
   * @return the attributes of this window
   */
  PronWindowAttributes getAttributes();

  /**
   * Sets the attributes of this window.
   * @param newAttributes The new attributes structure
   * @param mask The mask that specifies the attributes to be set
   */
  void setAttributes(PronWindowAttributes *newAttributes, unsigned int mask);

  /**
   * Subscribes to the events associated with the specified event mask
   * for the specified client.
   * @param client The client who subscribes to the events
   * @param mask The event mask
   */
  void selectInput(Client *client, unsigned int mask);

  /**
   * Raises this window (moves it on top of the screen).
   */
  void raise();

  /**
   * Destroys this window.
   */
  void destroy();

  /**
   * Moves this window of x pixels on the x-axis and y pixels on the y-axis.
   * @param dx The relative move on the x-axis
   * @param dy The relative move on the y-axis
   */
  void move(int dx, int dy);

  /**
   * Moves this window at (x,y) position, relative to its parent.
   * @param x The new x-coordinate, relative to the parent window
   * @param y The new y-coordinate, relative to the parent window
   */
  void moveTo(int x, int y);

  /**
   * Resizes a window.
   * @param width The new width
   * @param height The new height
   */
  void resize(int width, int height);

  /**
   * Delivers an event to this window.
   * @param e The event to deliver
   * @param size The size of the event
   */
  void deliverEvent(PronEvent *e, unsigned int size);

  /**
   * Delivers an event to this window and its immediate parent.
   * Suitable for window events (EV_WINDOW_CREATED, EV_EXPOSE...).
   * @param e The event to deliver
   * @param size The size of the event
   */
  void deliverWindowEvent(PronEvent *e, unsigned int size);

  /**
   * Delivers a event that needs to propagate up the window tree.
   * Suitable for device events (EV_POINTER_MOVED, EV_KEY_PRESSED...).
   * @param e The event to deliver
   * @param size The size of the event
   */
  void deliverDeviceEvent(PronEvent *e, unsigned int size);

  /**
   * Returns true when the window contains (x,y)
   * @param x The x-coordinate of the point to check
   * @param y The y-coordinate of the point to check
   * @return true when the window contains (x,y)
   */
  bool contains(int x, int y);

  /**
   * Returns true when the window and all its parents are mapped.
   * @return true if the window and all its parent are mapped.
   */
  bool realized();

  /**
   * Returns the x-coordinate of the top-left corner of the window.
   * @return the x-coordinate of the top-left corner of the window
   */
  int getX();

  /**
   * Sets the x-coordinate of the top-left corner of the window.
   * @param x The x-coordinate of the top-left corner of the window
   */
  void setX(int x);

  /**
   * Returns the y-coordinate of the top-left corner of the window.
   * @return the y-coordinate of the top-left corner of the window.
   */
  int getY();

  /**
   * Sets the y-coordinate of the top-left corner of the window.
   * @param y The y-coordinate of the top-left corner of the window
   */
  void setY(int y);

  /**
   * Sets a new parent to the window.
   * @param w The new parent window
   */
  void reparent(Window *w);

  /**
   * Returns true if the window accepts at least one of the given events
   * @param eventMask The event mask to check
   * @return true if the window accepts one of the events
   */
  bool acceptsEvents(int eventMask);

 private: 
  int x; /**< Top-left corner x-coordinate */
  int y; /**< Top-left corner y-coordinate */
  color_t bgColor; /**< Background color */
  bool isResizable; /**< Wheter the window is resizable or not */
  int maxWidth; /**< Maximum width */
  int maxHeight; /**< Maximum height */
  int minWidth; /**< Minimum width */
  int minHeight; /**< Minimum height */
  bool mapped; /**< Whether the window is mapped (showable on the screen) or not */
  int unmappedParents; /**< Number of unmapped parents. */
  vector<OtherClient> otherClients; /**< Other clients who have selected events on this window */

  /**
   * Returns true if w overlaps this window.
   * @param w The window to check
   * @return true if w overlaps this window
   */
  bool overlaps(Window *w);
};

#endif
