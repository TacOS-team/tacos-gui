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
      bool multiLine; /**< Whether the textbox can contain multiple lines or not. */

    public:
      /**
       * Constructor.
       */
      Textbox(bool multiLine = true);

      /**
       * Destructor.
       */
      ~Textbox();

      /**
       * The handle for an expose event
       */
      void handleEventExpose();

      /**
       * The handle for a key pressed event
       * @param e A pointer to the event (will give details of the key pressed)
       */
      void handleEventKeyPressed(pron::EventKeyPressed *keyPressed);

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
