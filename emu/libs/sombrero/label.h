#ifndef __LABEL_H__
#define __LABEL_H__
/**
 * @file label.h
 * Button class definition
 */

#include "widget.h"

namespace sombrero {

/**
 * Button class definition
 * @brief The Label widget
 */ 
class Label : public Widget {
 private:
  char *text;
  
 public:

  /**
   * Set the label of the text
   * @param text the new text
   */
  void setText(char *text);
  /**
   * The label constructor 
   * @param parent Parent Container
   */
  Label(Container *parent, char *text);
  /**
   * Label destructor
   */
  ~Label();
  /**
   * Label drawing stuff
   */
  void draw();

};

}

#endif//__LABEL_H__
