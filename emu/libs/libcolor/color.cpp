#include "color.h"

int pow(int a, int b) {
  int i;
  int res = a;
  if (b == 0) {
    return 1;
  }
  for (i = 1; i < b; i++) {
    res *= a;
  }
  return res;
}

Color::Color(float r, float g, float b)
    : r(r), g(g), b(b){
}

float Color::getR() {
  return this->r;
}

float Color::getG() {
  return this->g;
}

float Color::getB() {
  return this->b;
}

void Color::setR(float r) {
  this->r = r;
}

void Color::setG(float g) {
  this->g = g;
}

void Color::setB(float b) {
  this->b = b;
}

int Color::getRGB() {
  int pixel = 0;
  // Red
  pixel |= (int)(this->r * 0xFF);
  // Green
  pixel |= ((int)(this->g * 0xFF)) << 8;
  // Blue
  pixel |= ((int)(this->b * 0xFF)) << 16;
  // The rest of the pixel
  pixel &= 0x00FFFFFF;
  return pixel;
}
