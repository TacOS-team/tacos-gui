#include "colorpicker.h"
#include "button.h"
#include "widget.h"

namespace sombrero {

//class Widget;

enum COLOR_COMPONENT {
  C_RED,
  C_GREEN,
  C_BLUE
};

class ColorButton : public Button {
 private:
  Color c;
  ColorPicker *cp;
  int colorComponent;
 protected:
 public:
  ColorButton(const Color &c, ColorPicker *cp, int colorComponent) 
      : Button(), c(c), cp(cp), colorComponent(colorComponent) {
    this->setBGColor(c);
  }
  void handleMouseDown(sombrero::MouseButton b __attribute__((unused)), int x __attribute__((unused)), int y __attribute__((unused))) {
    cp->notifyCurrentColor(this->c, colorComponent);
  }
};

void ColorPicker::setCurrentColor(Color c) {
  this->c = c;
  this->lcolor.setBGColor(this->c);
  this->lcolor.draw();
}

void ColorPicker::notifyCurrentColor(Color c, int colorComponent) {

  switch (colorComponent) {
    case C_RED: 
      this->c.setR(c.getR());
      break;
    case C_GREEN:
      this->c.setG(c.getG());
      break;
    case C_BLUE:
      this->c.setB(c.getB());
      break;
  }

  this->lcolor.setBGColor(this->c);
  this->lcolor.draw();
  this->colorChanged(this->c);
}

void ColorPicker::init() {
  int i;
  unsigned char c;
  // initializes the Color button table
  this->bcolors = new ColorButton*[(nbColors + 1) * 3];
  for (i = 0; i <= this->nbColors; ++i) {
    if (i < this->nbColors) {
      c = i * (255 / this->nbColors);
    } else {
      c = 255;
    }
    // Creates the buttons
    Color col(c, 0, 0);
    this->bcolors[i] = new ColorButton(col, this, C_RED); // red
    Color col2(0, c, 0);
    this->bcolors[i + this->nbColors] = new ColorButton(col2, this, C_GREEN); // green
    Color col3(0, 0, c);
    this->bcolors[i + 2 * this->nbColors] = new ColorButton(col3, this, C_BLUE); // blue
    // Adds the buttons to the color picker
    this->attach(this->bcolors[i], i, 0, 1, 1);
    this->attach(this->bcolors[i + this->nbColors], i, 1, 1, 1);
    this->attach(this->bcolors[i + 2 * this->nbColors], i, 2, 1, 1);
  }
  // Adds the color sum into the grid
  this->attach((Widget *)&this->lcolor, i, 0, this->nbColors / 3, 3);

}

ColorPicker::~ColorPicker() {
  int i;
  // Freeing colors
  for (i = 0; i < this->nbColors * 3; ++i) {
    delete this->bcolors[i];
  }
  // Freeing color table
  delete this->bcolors;
}

ColorPicker::ColorPicker(unsigned int nbColors) 
    : Grid(), nbColors(nbColors), lcolor("") {
  init();
}

} // namespace sombrero
