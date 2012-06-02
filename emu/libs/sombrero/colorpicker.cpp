#include "colorpicker.h"
#include "button.h"

namespace sombrero {

class Widget;

class ColorButton : public Button{
 private:
  Color c;
  ColorPicker *cp;
 protected:
 public:
  ColorButton(const Color &c, ColorPicker *cp) 
      : Button(), c(c), cp(cp) {
    this->setBGColor(c);
  }
  void handleMouseDown(sombrero::MouseButton b __attribute__((unused)), int x __attribute__((unused)), int y __attribute__((unused))) {
    cp->setCurrentColor(this->c);
  }
};

void ColorPicker::setCurrentColor(Color c) {
  this->c = c;
}

void ColorPicker::init() {
  int i;
  unsigned char c;
  // initializes the Color button table
  this->bcolors = new ColorButton*[nbColors * 3];
  for (i = 0; i < this->nbColors; ++i) {
    c = i * (255 / this->nbColors);
    printf("c %d\n", c);
    // Creates the buttons
    Color col(c, 0, 0);
    this->bcolors[i] = new ColorButton(col, this); // red
    Color col2(0, c, 0);
    this->bcolors[i + this->nbColors] = new ColorButton(col2, this); // green
    Color col3(0, 0, c);
    this->bcolors[i + 2 * this->nbColors] = new ColorButton(col3, this); // blue
    // Adds the buttons to the color picker
    this->attach((Widget *)this->bcolors[i], i, 0, 1, 1);
    this->attach((Widget *)this->bcolors[i + this->nbColors], i, 1, 1, 1);
    this->attach((Widget *)this->bcolors[i + 2 * this->nbColors], i, 2, 1, 1);
  }
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
    : Grid(), nbColors(nbColors) {
  init();
}

} // namespace sombrero
