#include "colorpicker.h"
#include "button.h"

namespace sombrero {

class Widget;

class ColorButton : Button{
 private:
  Color c;
  ColorPicker *cp;
 protected:
 public:
  ColorButton(int r, int g, int b, ColorPicker *cp) 
      : Button(), c(r, g, b), cp(cp) {
  }
  void handleMouseDown(sombrero::MouseButton b, int x, int y) {
    cp->setCurrentColor(this->c);
    printf("LOLLLLLLL 0x%x\n", this->c.getRGB24());
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
    // Creates the buttons
    this->bcolors[i] = new ColorButton(c, 0, 0, this); // red
    this->bcolors[i + this->nbColors] = new ColorButton(0, c, 0, this); // green
    this->bcolors[i + 2 * this->nbColors] = new ColorButton(0, 0, c, this); // blue
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
