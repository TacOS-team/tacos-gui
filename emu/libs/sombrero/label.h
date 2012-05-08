#ifndef __LABEL_H__
#define __LABEL_H__
/**
 * @file label.h
 * Button class definition
 */

#include "widget.h"
#include <string>

namespace sombrero {

/**
 * Button class definition
 * @brief The Label widget
 */ 
class Label : public Widget {
 private:
  unsigned char fontSize;
  std::string text;
  
 public:

  /**
   * Set the label of the text
   * @param text the new text
   */
  void setText(const std::string text);
  /**
   * Set the label of the text
   * @param text the new text
   */
  void setText(const char *text);
  /**
   * The label constructor 
   * @param parent Parent Container
   * @param text The text of the label
   */
  Label(Container *parent, const char *text);
  /**
   * The label constructor
   * @param text The text of the label
   */
  Label(const char *text);
  /**
   * The label constructor 
   * @param parent Parent Container
   * @param text The text of the label
   */
  Label(Container *parent, const std::string text);
  /**
   * The label constructor
   * @param text The text of the label
   */
  Label(const std::string text);
  /**
   * Label destructor
   */
  ~Label();
  /**
   * Initialize the label
   */
  void init();
  /**
   * Label drawing stuff
   */
  void draw();

};

} // namespace sombrero

#endif//__LABEL_H__
