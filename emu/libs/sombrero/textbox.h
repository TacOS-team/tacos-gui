#ifndef __TEXTBOX_H__
#define __TEXTBOX_H__

/**
 * @file textbox.h
 * Textbox class definition
 */

#include <string>

#include "widget.h"

using namespace std;

namespace sombrero {

class Textbox : public Widget {
 private:
  string text; /**< Text buffer */
  unsigned int display; /**< Position of the first displayed character */
  unsigned int cursor; /**< Cursor position in the buffer */

 public:
  /**
  * Constructor.
  * @param parent The parent widget, must be a container.
  */
  Textbox(Container *parent);

  /**
  * Destructor.
  */
  ~Textbox();

  /**
  * Event handler.
  * @param e The event to handle
  */
  void handleEvent(pron::PronEvent *e);

  /**
  * Draws the textbox.
  */
  void draw();

  static const int MARGIN_TOP = 4;
  static const int MARGIN_RIGHT = 4;
  static const int MARGIN_BOTTOM = 4;
  static const int MARGIN_LEFT = 4;
};

} // namespace sombrero

#endif // __TEXTBOX_H__