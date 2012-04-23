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
   * Container constructor
   */
  Container(Container *parent, int x, int y, int width, int height);
  /**
   * Container constructor
   */
  Container(int x, int y, int width, int height);
  /**
   * Container destructor
   */
  ~Container();
  
 public:
  /**
   * Adds a widget in the children vector
   * @param widget Pointer to the widget to add
   */
  void add(Widget *widget);
  /**
   * Remove a widget from the children vector
   * @param widget Pointer to the widget to remove
   */
  void remove(Widget *widget);
  /**
   * Gets a copy of the children vector
   * @return The copied vector of children widgets
   */
  std::vector<Widget*> getChildren();
  
};

} //namespace sombrero

#endif//__CONTAINER_H__
