#ifndef __CLIP_RECT_H__
#define __CLIP_RECT_H__

#include <vector>
#include <window.h>

using namespace std;

class ClipRect {
public:
  int x, y;
  int width, height;

  ClipRect(int x, int y, int width, int height);
  ClipRect(Window *w);

  vector<ClipRect*> split(ClipRect *obscurer);

  bool contains(int x, int y);
};

#endif
