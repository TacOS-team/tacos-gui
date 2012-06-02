#ifndef __GRID_H__
#define __GRID_H__
/**
 * @file grid.h
 * Grid class definition
 */

#include "container.h"
#include "window.h"
#include <algorithm>

class Container;

namespace sombrero {

enum PositionType {
  POS_LEFT,
  POS_RIGHT,
  POS_TOP,
  POS_BOTTOM,
};

/**
 * Grid class definition. A grid is a widget containing several other widgets
 *    as a grid.
 */
class Grid : public Container {
  
 protected:
  /**
   * Wrapper to stock the widgets with additionnal informations
   */
  class widgetWrapper {
   public:
    /**
     * The widget
     */
    Widget *widget;
    /**
     * The column span
     */
    int width;
    /**
     * The line span
     */
    int height;
    /**
     * The x position in the grid
     */
    int x;
    /**
     * The y position in the grid
     */
    int y;

    /**
     * Constructs the wrapper with all informations sets to 1
     */
    widgetWrapper(Widget* w);
  };

  /**
   * A line is a vector of wrappers
   */
  typedef vector < widgetWrapper* > line_t;
  /**
   * A column is a vector of lines
   */
  typedef vector < line_t > column_t;

  /**
   * The grid. A vector of vector of wrappers. If a wrappers spans on several
   *  lines or columns, the same wrapper pointer will be in several positions in the grid.
   *  That is why the wrapper contains the x,y positions and the span informations.
   */
  column_t widgetsTab;

  /**
   * The number of columns of the grid (to avoid to calculate it each time)
   */
  size_t nbColumns;

  /**
   * Initialises the grid
   */
  void init();

  /**
   * Finds a widget in the grid and returns its position
   * @param w The widget to insert
   * @param x The x position where the widget has been found
   * @param y The y position where the widget has been found
   * @return true if the widget has been found
   */
  bool find(Widget * w, size_t &x, size_t &y);

  /**
   * Finds a widget in the grid and returns its position
   * @param The initialised wrapper to insert
   */
  virtual void attach(widgetWrapper *wrapper);

 public:
  /**
   * Default constructor
   */
  Grid();

  /**
   * Adds a widget on the last line of the grid
   * @param widget The widget to insert
   */
  virtual void add(Widget *widget);
  /**
   * Removes a widget 
   * @TODO unimplemented
   * @param widget The widget to remove
   */
  virtual void remove(Widget *widget);
  /**
   * Adds a widget at the position (x,y) with specified width and height
   * @param widget The widget to insert
   * @param x      The x position where the widget will be inserted
   * @param y      The y position where the widget will be inserted
   * @param width  The number of columns to span the widget
   * @param height The number of lines to span the widget
   */
  virtual void attach(Widget *child, int x, int y, int width, int height);
  /**
   * Adds a widget at the position (x,y) with specified width and height
   * @param child   The widget to insert
   * @param sibling The widget where the new widget will be placed near
   * @param side    Position where the child will be placed relatively to sibling
   *                  (POS_RIGHT, POS_LEFT, POS_TOP, POS_BOTTOM)
   * @param width   The number of columns to span the widget
   * @param height  The number of lines to span the widget
   */
  virtual void attachNextTo(Widget *child, Widget *sibling, PositionType side, int width, int height);

  /**
   * Adds an new empty line at the end of the grid
   */
  void newLine();

  /**
   * Inserts a row at the "position" position
   * @param position The position to insert the row
   */
  void insertRow (int position);
  /**
   * Inserts several rows at the "position" position
   * @param position The position to insert the rows
   * @param nb       The number of rows to insert
   */
  void insertRows(int position, size_t nb);

  /**
   * Inserts a column at the "position" position
   * @param position The position to insert the column
   */
  void insertColumn (int position);
  /**
   * Inserts several columns at the "position" position
   * @param position The position to insert the column
   * @param nb       The number of columns to insert
   */
  void insertColumns(int position, size_t nb);

  /**
   * Drawing stuff
   */
  virtual void draw();

  /**
   * Updates the display of the widgets. Resize and replace correctly
   */
  void execUpdate();
  /**
   * Sets the parent container
   * @param Pointer to the parent container
   */
  virtual void setParent(Widget *parent);
};

} //namespace sombrero

#endif//__GRID_H__
