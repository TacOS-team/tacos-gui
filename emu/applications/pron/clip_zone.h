#ifndef __CLIP_ZONE_H__
#define __CLIP_ZONE_H__

#include <clip_rect.h>
#include <vector>
#include <window.h>

using namespace std;

class ClipZone {
public:
  ClipZone(int x, int y, int width, int height);
  ClipZone(Window *w);

  bool contains(int x, int y);
  bool contains(int x1, int y1, int x2, int y2);

  void print();

private:
  vector<ClipRect*> clipRects;
};

#endif
