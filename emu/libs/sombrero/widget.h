#ifndef __WIDGET_H__
#define __WIDGET_H__
/**
 * @file widget.h
 * Widget class definition
 */

#include "pronlib.h"
#include "sigslot.h"
#include <sys/time.h>

using namespace sigslot;

namespace sombrero {

enum MouseButton {
  rightButton,
  leftButton,
  middleButton,
};

class Container;

/**
 * Widget class definition. A widget is something dealing with 
 * graphic and events
 */
class Widget : public has_slots<> {

 private:
  Widget *parent; /**< The parent widget */
  pron::PronWindowAttributes attributes; /**< Current attributes (x, y, width, height...) */
  pron::PronWindowAttributes lastAttributes; /**< Last attributes received/sent from/to pron */
  bool active; /**< Dealing with activable widgets */
  bool visible; /**< The widget has to be shown */
  uint32_t eventMask; /**< The event mask */
  uint32_t dontPropagateEventMask; /**< The event mask to don't propagate */
  pron::EventMouseButton oldButtonsState;/**< the buttons state of the last mouse event */
  bool isUpdating;/**< True when the window is beeing updated */
  struct timeval lastClick;/**< The time of the last click */

 protected:
  pron::GC fgGC; /**< The foreground GC */
  pron::GC bgGC; /**< The background GC */

  /**
   * The pronWindow associated with the widget.
   * Has to be reparented when the widget is reparented.
   */
  pron::Window pronWindow;
  /**
   * Widget default constructor that does nothing.
   */
  Widget();
  /**
   * Widget destructor. Destroys the pron top window
   */
  ~Widget();
  /**
   * initialize the widget
   */
  void init();
  /**
   * Function called when a client clicks on the widget
   * @param button The clicked button (rightButton, middleButton or leftButton)
   * @param x the x position of the mouse
   * @param y the y position of the mouse
   */
  virtual void handleMouseDown(MouseButton button, int x, int y);
  /**
   * Function called when a client double clicks on the widget
   * @param x the x position of the mouse
   * @param y the y position of the mouse
   */
  virtual void handleDoubleClick(int x, int y);
  /**
   * Function called when a client clicks on the widget
   * @param x the x position of the mouse
   * @param y the y position of the mouse
   */
  virtual void handleClick(int x, int y);
  /**
   * Function called when a client clicks on the widget
   * @param button The clicked button (rightButton, middleButton or leftButton)
   */
  virtual void handleMouseReleased(MouseButton button, int x, int y);
  /**
   * returns true if the pron windw has been created
   */
  bool isPronWindowCreated();

 public:
  // Signals
  signal0<> resized;
  /**
   * Signal sent when the mouse moves.
   * <xMove, yMove, relativeXPosition, relativeYPosition>
   */
  signal4<int, int, int, int> mouseMoved;
  /**
   * Subscribes to new event
   * @param eventMask The event mask to add (only one event at once)
   */
  void subscribeEvent(uint32_t eventMask);
  /**
   * Unsubscribes to new event
   * @param eventMask The event mask to add (only one event at once)
   */
  void unsubscribeEvent(uint32_t eventMask);
  /**
   * Don't propagate new event
   * @param eventMask The event mask to propagate (only one event at once)
   */
  void dontPropagateEvent(uint32_t eventMask);
  /**
   * Propagates the event
   * @param eventMask The event mask to propagate (only one event at once)
   */
  void propagateEvent(uint32_t eventMask);
  // Getters and setters
  /**
   * Sets the parent container
   * @param Pointer to the parent container
   */
  virtual void setParent(Widget *parent);
  /**
   * Gets x
   * @return x
   */
  int getX();
  /**
   * Sets the x
   * @param x
   */
  void setX(int x);
  /**
   * Gets y
   * @return y
   */
  int getY();
  /**
   * Sets y
   * @param y
   */
  void setY(int y);
  /**
   * Sends the new position to pron
   */
  void updatePronPosition();
  /**
   * Sends the new window bgColor to pron
   */
  void updateBGColor();
  /**
   * Gets the width
   * @return width
   */
  int getWidth();
  /**
   * Sets the width
   * @param width
   */
  void setWidth(int width);
  /**
   * Gets the height
   * @return height
   */
  int getHeight();
  /**
   * Sets the height
   * @param height
   */
  void setHeight(int height);
  /**
   * Sends the new size to pron
   */
  void updatePronSize();
  /**
   * Updates the widgets to pron
   */
  void update();
  /**
   * the update function to overload
   */
  virtual void execUpdate();
  /**
   * Gets active
   * @return active
   */
  bool isActive();
  /**
   * Sets active
   * @param active
   */
  void setActive(bool active);
  /**
   * Gets visible
   * @return visible
   */
  bool isVisible();
  /**
   * Sets visible
   * @param visible
   */
  void setVisible(bool visible);
  /**
   * Put here widgets drawing stuff
   */
  virtual void draw();
  /**
   * Clears the widget
   */
  void clear();
  /**
   * Handle for a windowCreated event
   */
  virtual void handleEventWindowCreated();
  /**
   * Handle for an expose event
   */
  virtual void handleEventExpose();
  /**
   * Handle for a pointerMoved event
   */
  virtual void handleEventPointerMoved(pron::EventPointerMoved * e);
  /**
   * Handle for a mouseButton event
   * @param e A pointer to the event that accured
   */
  virtual void handleEventMouseButton(pron::EventMouseButton * e);
  /**
   * Handle for a keyReleased event
   */
  virtual void handleEventKeyReleased();
  /**
   * Handle for a destroyWindow event
   */
  virtual void handleEventDestroyWindow();
  /**
   * Handle for a resizeWindow event
   */
  virtual void handleEventResizeWindow(int width, int height);
  /**
   * Gets parent container
   * @return The parent Container
   */
  Widget* getParent();
  /**
   * Handle for a keyPressed event
   * @param e A pointer to the event that occured
   */
  virtual void handleEventKeyPressed(pron::EventKeyPressed *e);
  /**
   * Set the foreground color
   */
  void setFGColor(const Color &c);
  /**
   * Set the background color
   */
  void setBGColor(const Color &c);

};

} // namespace sombrero

#endif//__WIDGET_H__
