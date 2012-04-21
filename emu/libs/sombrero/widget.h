#ifndef __WIDGET_H__
#define __WIDGET_H__
/**
 * @file widget.h
 * Widget class definition
 */

#include "pronlib.h"

class Application;
class Container;

namespace sombrero {

/**
 * Widget class definition. A widget is something dealing with 
 * graphic and events
 */
class Widget {

 private:
  int width; /**< Width */
  int height; /**< Height */
  bool active; /**< Dealing with activable widgets */
  bool visible; /**< The widget has to be shown */
  /**
   * The to window of the widget. Has to be reparented 
   * when the widget is.
   */
  pron::Window topWindow; 

 protected:
  /**
   * Gets the top window of the widget
   * @see topWindow
   * @return The pron window
   */
  pron::Window getTopWindow();
  
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

}

} // namespace sombrero

#endif//__WIDGET_H__
