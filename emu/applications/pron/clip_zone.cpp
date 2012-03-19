#include <clip_zone.h>
#include <screen.h>
#include <cstdio>

ClipZone::ClipZone(int x, int y, int width, int height) {
  // Clipping zone containing only 1 rectangle
  ClipRect *clip = new ClipRect(x, y, width, height);
  this->clipRects.push_back(clip);
}

ClipZone::ClipZone(Window *w) {
  // Initial rectangle
  ClipRect *clip = new ClipRect(w);
  this->clipRects.push_back(clip);

  // Restrict this rectangle to the part visible through all its parents
  Window *parent, *sibling;
  parent = w;
  while (parent != w->screen->root) {
    parent = parent->parent;

    // X1
    if (parent->x > clip->x) {
      clip->x = parent->x;
      clip->width -= (parent->x - clip->x);
    }

    // Y1
    if (parent->y > clip->y) {
      clip->y = parent->y,
      clip->height -= (parent->y - clip->y);
    }

    // X2
    if (parent->x + parent->width < clip->x + clip->width) {
      clip->width -= (clip->x + clip->width - (parent->x + parent->width));
    }

    // Y2
    if (parent->y + parent->height < clip->y + clip->height) {
      clip->height -= (clip->y + clip->height - (parent->y + parent->height));
    }
  }

  // If it's still visible
  if (clip->width > 0 || clip->height > 0) {
    // Break this rectangle into pieces not obscured by other windows
    parent = w;
    while (parent != w->screen->root) {
      w = parent;
      parent = w->parent;
      sibling = w->nextSibling;
      
      for (; sibling != NULL; sibling = sibling->nextSibling) {
        if (sibling->mapped) {
          ClipRect obscurer(sibling);
          vector<ClipRect*> oldClipRects = this->clipRects;
          this->clipRects.clear();
          for (unsigned int i = 0; i < oldClipRects.size(); i++) {
            vector<ClipRect*> splittedRect = oldClipRects[i]->split(&obscurer);
            this->clipRects.insert(this->clipRects.begin(), splittedRect.begin(), splittedRect.end());
            delete oldClipRects[i];
          }
        }
      }
    }
  }
}

bool ClipZone::contains(int x, int y) {
  bool inZone = false;
  
  for (unsigned int i = 0; i < this->clipRects.size() && !inZone; i++) {
    inZone = inZone || this->clipRects[i]->contains(x, y);
  }

  return inZone; 
}

void ClipZone::print() {
  printf("########## CLIP ZONE ##########\n");
  for (unsigned int i = 0; i < this->clipRects.size(); i++) {
    printf("x: %d  y: %d  w: %d  h: %d\n", this->clipRects[i]->x, this->clipRects[i]->y, this->clipRects[i]->width, this->clipRects[i]->height);
  }
  printf("###############################\n");
}