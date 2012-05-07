#ifndef __GRID_H__
#define __GRID_H__
/**
 * @file canvas.h
 * Canvas class definition
 */

#include "container.h"
#include <algorithm>

#define SOMBRERO_CANVAS_DEPTH 24

class Container;

namespace sombrero {

/**
 * Grid class definition. A grid is a widget containing several other widgets
 *    as a grid.
 */
class Grid : public Container, public has_slots<> {
  
 protected:
  class widgetWrapper {
   public:
    Widget * widget;
    int width;
    int height;
    int x;
    int y;

    widgetWrapper(Widget* w);
  };

  typedef vector < widgetWrapper* > line_t;
  typedef vector < line_t         > column_t;

  column_t widgetsTab;

  unsigned short nbColumns;

  void update();

  void parentResized();

 public:
  Grid(Container *parent);

  void add(Widget* widget);

  void newLine();

  /**
   * Drawing stuff
   */
  void draw();
};

} //namespace sombrero

#endif//__GRID_H__
