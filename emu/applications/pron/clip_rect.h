#ifndef __CLIP_RECT_H__
#define __CLIP_RECT_H__

#include <vector>
#include <window.h>

using namespace std;

/**
 * Clip rectangle.
 * Defines a rectangle that belongs to the clipping zone.
 * @todo XXX public members bourrin
 */
class ClipRect {
public:
  int x; /**< Top-left corner x-coordinate */
  int y; /**< Top-left corner y-coordinate */
  int width; /**< Width of the rectangle */
  int height; /**< Height of the rectangle */

  /**
   * Constructor.
   * @param x The x-coordinate of the top-left corner of the rectangle
   * @param y The y-coordinate of the top-left corner of the rectangle
   * @param width The width of the rectangle
   * @param height The height of the rectangle
   */
  ClipRect(int x, int y, int width, int height);

  /**
   * Constructor.
   * @param w The window representing the rectangle
   */ 
  ClipRect(Window *w);

  /**
   * Splits the clip rectangle into new smallest rectangles that are not
   * obscured by the given rectangle
   * @param obscurer The rectangle that obscures the initial clip rectangle
   */
  vector<ClipRect*> split(ClipRect *obscurer);

  /**
   * Returns true if the clip rectangle contains (x, y)
   * @return true if the clip rectangle contains (x, y)
   */
  bool contains(int x, int y);
};

#endif
