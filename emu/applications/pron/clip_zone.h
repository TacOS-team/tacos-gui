#ifndef __CLIP_ZONE_H__
#define __CLIP_ZONE_H__

#include <clip_rect.h>
#include <vector>
#include <window.h>

using namespace std;

/**
 * Clipping zone.
 * Defines a set of rectangles we can draw into.
 * Any point that is not in the clipping zone won't be drawn.
 */
class ClipZone {
public:
  /**
   * Constructor.
   * Creates an initial clipping zone containing the given rectangle.
   * @param x The x-coordinate of the top-left corner of the rectangle
   * @param y The y-coordinate of the top-left corner of the rectangle
   * @param width The width of the rectangle
   * @param height The height of the rectangle
   */
  ClipZone(int x, int y, int width, int height);

  /**
   * Constructor.
   * Creates a clipping zone containing the visible areas of the given window.
   * @param w The window for which to set the clipping
   */
  ClipZone(Window *w);

  /**
   * Returns true if the clipping zone contains (x, y).
   * @param x The x-coordinate of the point
   * @param y The y-coordinate of the point
   * @return true if the clipping zone contains (x, y)
   */
  bool contains(int x, int y);

  /**
   * Returns true if the clipping zone contains the rectangle
   * ((x1, y1), (x2, y2)).
   * @param x1 The x-coordinate of the top-left corner of the rectangle
   * @param y1 The y-coordinate of the top-left corner of the rectangle
   * @param x2 The x-coordinate of the bottom-right corner of the rectangle
   * @param y2 The y-coordinate of the bottom-right corner of the rectangle
   * @return true if the clipping zone contains the rectangle
   * ((x1, y1), (x2, y2))
   */
  bool contains(int x1, int y1, int x2, int y2);

  /**
   * Prints the clipping zone (for debugging purposes).
   */
  void print();

private:
  vector<ClipRect*> clipRects;
};

#endif
