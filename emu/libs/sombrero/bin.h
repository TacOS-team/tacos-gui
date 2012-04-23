#ifndef __BIN_H__
#define __BIN_H__
/**
 * @file container.h
 * Bin class definition
 */

#include "container.h"
#include <vector>

class Container;

namespace sombrero {

/**
 * Bin class definition. A container is a widget containing 
 * one widget
 */
class Bin : public Container {
  
 private:
  int full;
  
 protected:
  /**
   * Bin constructor
   * @param parent The parent container
   * @param x
   * @param y
   * @param width
   * @param height
   */
  Bin(Container *parent, int x, int y, int width, int height);
  /**
   * Bin constructor
   * @param x
   * @param y
   * @param width
   * @param height
   */
  Bin(int x, int y, int width, int height);
  /**
   * Bin destructor
   */
  ~Bin();
  
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

};

} //namespace sombrero

#endif//__BIN_H__
