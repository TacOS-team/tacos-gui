#include <window.h>
#include <screen.h>
#include <client.h>
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <drawable.h>

Drawable::Drawable(int type, Screen *screen, int id, Client *creator, int width, int height) {
  this->type = type;
  this->screen = screen;
  this->id = id;
  this->creator = creator;
  this->width = width;
  this->height = height;
}

Drawable::~Drawable() {
  // Destruction
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