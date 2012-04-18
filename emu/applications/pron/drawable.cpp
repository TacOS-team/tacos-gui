#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <client.h>
#include <drawable.h>
#include <screen.h>
#include <window.h>

Drawable::Drawable(int type, Screen *screen, int id, Client *creator, int width, int height) {
  this->type = type;
  this->screen = screen;
  this->id = id;
  this->creator = creator;
  this->width = width;
  this->height = height;

  // Add the new drawable to the screen's list of drawables
  screen->addDrawable(this);
}

Drawable::~Drawable() {
  // Destruction
  this->getScreen()->drawables.erase(std::find(this->getScreen()->drawables.begin(), this->getScreen()->drawables.end(), (Drawable*) this));
}

int Drawable::getType() {
  return this->type;
}

Screen* Drawable::getScreen() {
  return this->screen;
}

unsigned int Drawable::getId() {
  return this->id;
}

Client* Drawable::getCreator() {
  return this->creator;
}

int Drawable::getWidth() {
  return this->width;
}

void Drawable::setWidth(int width) {
  this->width = width;
}

int Drawable::getHeight() {
  return this->height;
}

void Drawable::setHeight(int height) {
  this->height = height;
}

void Drawable::reduce(int &x, int &y, int &width, int &height) {
  if (x < 0) {
    width += x;
    x = 0;
  }

  if (y < 0) {
    height += y;
    y = 0;
  }

  if (x + width > this->getWidth()) {
    width = this->getWidth() - x;
  }

  if (y + height > this->getHeight()) {
    height = this->getHeight() - y;
  }
}
