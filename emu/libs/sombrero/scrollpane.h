#ifndef __SCROLLPANE_H__
#define __SCROLLPANE_H__
/**
 * @file scrollpane.h
 * Button class definition
 */

#include "bin.h"
#include "vscrollbar.h"

namespace sombrero {

/**
 * ScrollPane class definition
 * @brief The Scrollpane container
 */ 
class ScrollPane : public Bin {

 protected:
  VScrollBar scrollBar;
 public:
  ScrollPane();
};

} // namespace sombrero

#endif//__SCROLLPANE_H__
