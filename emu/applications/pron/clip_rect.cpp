#include <clip_rect.h>

ClipRect::ClipRect(int x, int y, int width, int height) {
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
}

ClipRect::ClipRect(Window *w) {
  this->x = w->getX();
  this->y = w->getY();
  this->width = w->getWidth();
  this->height = w->getHeight();
}

vector<ClipRect*> ClipRect::split(ClipRect *obscurer) {
  vector<ClipRect*> splittedRect;

  // No overlap
  if (this->x >= obscurer->x + obscurer->width ||
      this->y >= obscurer->y + obscurer->height ||
      this->x + this->width <= obscurer->x ||
      this->y + this->height <= obscurer->y) {
    splittedRect.push_back(new ClipRect(this->x, this->y, this->width, this->height));
    return splittedRect;
  }

  // Determine the zones that are still visible
  if (this->y < obscurer->y) {
    // Visible zone on top
    splittedRect.push_back(new ClipRect(this->x, this->y, this->width, obscurer->y - this->y));
  }

  if (this->y + this->height > obscurer->y + obscurer->height) {
    // Visible zone at bottom
    splittedRect.push_back(new ClipRect(this->x, obscurer->y + obscurer->height, this->width, this->y + this->height - (obscurer->y + obscurer->height)));
  }

  if (this->x < obscurer->x) {
    // Visible zone at left
    splittedRect.push_back(new ClipRect(this->x, this->y, obscurer->x - this->x, this->height));
  }

  if (this->x + this->width > obscurer->x + obscurer->width) {
    // Visible zone at right
    splittedRect.push_back(new ClipRect(obscurer->x + obscurer->width, this->y, this->x + this->width - (obscurer->x + obscurer->width), this->height));
  }

  return splittedRect;
}

bool ClipRect::contains(int x, int y) {
  return (x >= this->x && x < this->x + this->width && y >= this->y && y < this->y + this->height);
}
