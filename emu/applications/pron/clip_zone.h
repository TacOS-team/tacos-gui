#ifndef __CLIP_ZONE_H__
#define __CLIP_ZONE_H__

#include <clip_rect.h>
#include <vector>
#include <window.h>

using namespace std;

class ClipZone {
public:
  ClipZone(Window *w);

  bool contains(int x, int y);

private:
  vector<ClipRect*> clipRects;
};

#endif
