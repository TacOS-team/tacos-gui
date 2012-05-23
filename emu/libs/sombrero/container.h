#ifndef __CONTAINER_H__
#define __CONTAINER_H__
/**
 * @file container.h
 * Container class definition
 */

#include "widget.h"
#include <vector>

class Widget;

namespace sombrero {

/**
 * Container class definition. A container is a widget containing 
 * other widgets 
 */
class Container : public Widget {
  
 private:
  vector<Widget*> children; /**< collection of widgets */
  
 protected:
  /**
   * Container default constructor that does nothing.
   */
  Container();
  /**
   * Container destructor
   */
  ~Container();
  
 public:
  /**
   * Adds a widget in the children vector
   * @param widget Pointer to the widget to add
   */
  virtual void add(Widget *widget);
  /**
   * Remove a widget from the children vector
   * @param widget Pointer to the widget to remove
   */
  virtual void remove(Widget *widget);
  /**
   * Draw children widgets
   */
  void drawChildren();
  /**
   * Updates the container and its contents
   */
  virtual void update();
  /**
   * Draws the window
   */
  virtual void draw();
  /**
   * Gets a copy of the children vector
   * @return The copied vector of children widgets
   */
  std::vector<Widget*> getChildren();
  
};

} //namespace sombrero

#endif//__CONTAINER_H__
