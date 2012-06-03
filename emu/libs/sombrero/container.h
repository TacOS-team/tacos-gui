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
  virtual void add(Widget *widget) = 0;
  /**
   * Remove a widget from the children vector
   * @param widget Pointer to the widget to remove
   */
  virtual void remove(Widget *widget) = 0;
  /**
   * 
   */
  void setParent(Widget *widget);
  /**
   * Gets a copy of the children vector
   * @return The copied vector of children widgets
   */
  virtual std::vector<Widget*> getChildren() = 0;
  
};

} //namespace sombrero

#endif//__CONTAINER_H__
