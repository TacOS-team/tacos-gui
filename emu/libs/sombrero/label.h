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
   * @param text The text of the label
   */
  Label(Container *parent, char *text);
  /**
   * The label constructor
   * @param text The text of the label
   */
  Label(char *text);
  /**
   * Label destructor
   */
  ~Label();
  /**
   * Label drawing stuff
   */
  void draw();

};

} // namespace sombrero

#endif//__LABEL_H__
