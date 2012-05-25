#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__
/**
 * @file checkbox.h
 * Checkbox class definition
 */

#include "widget.h"
#include "pronlib.h"

namespace sombrero {

class Container;

/**
 * Checkbox class definition
 * @brief The Checkbox widget
 */ 
class Checkbox : public Widget {
  private:
    pron::GC gc; /**< specific GC for displaying red or green regarding to the state */
    pron::PronGCValues gcValues; /**< values of the gc */
    
  protected:
    bool state; /**< The state of the box (true if checked) */

  public:
    // Signals
    signal0<> clicked;
    /**
     * The checkbox constructor 
     */
    Checkbox();
    /**
     * checkbox destructor
     */
    ~Checkbox();

    /**
     * Redefined method for handling a mouseButton event
     * @param e A pointer to the event that occured
     */
    void handleEventMouseButton(pron::EventMouseButton *e) ;

    /**
     * checkbox drawing stuff
     */
    void draw();
    
    /**
     * Get the state of the checkbox, true if checked, else false
     * @return the state of the checkbox
     */
    bool isChecked();

};

} // namespace sombrero

#endif//__CHECKBOX_H__
