#ifndef __WIDGET_H__
#define __WIDGET_H__
/**
 * @file widget.h
 * Widget class definition
 */

#include "pronlib.h"

namespace sombrero {

class Container;

/**
 * Widget class definition. A widget is something dealing with 
 * graphic and events
 */
class Widget {

 private:
  Container *parent; /**< The parent widget */
  int x; /**< x coordinate */
  int y; /**< y coordinate */
  int width; /**< Width */
  int height; /**< Height */
  bool active; /**< Dealing with activable widgets */
  bool visible; /**< The widget has to be shown */

 protected:
  /**
   * The to window of the widget. Has to be reparented 
   * when the widget is.
   */
  pron::Window topWindow; 
  /**
   * Widget constructor. With given parent container
   * @param parent The parent container
   */
  Widget(Container *parent);
  /**
   * Widget destructor. Destroys the pron top window
   */
  ~Widget(); 
  // Getters and setters
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
  
 public:
  // Getters and setters
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
  virtual void draw() = 0;
  /**
   * Gets parent container
   * @return The parent Container
   */
  Container* getParent();

};

} // namespace sombrero

#endif//__WIDGET_H__
