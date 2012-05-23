#ifndef __RADIO_BUTTON_H__
#define __RADIO_BUTTON_H__

/**
 * @file radioButton.h
 * RadioButton class definition
 */

#include "pronlib.h"
#include "checkbox.h"
#include "radioButtonGroup.h"


namespace sombrero {

class RadioButtonGroup;

class RadioButton : public Checkbox {

  private :
    RadioButtonGroup *group; /**< The group of the radioButton */

  public :
    // Signals
    signal0<> clicked;

    /**
     * RadioButton constructor
     */
    RadioButton ();
    
    /**
     * RadioButton constructor
     * @param group Group containing the radioButton
     */
    RadioButton (RadioButtonGroup *group);
    
    /**
     * RadioButton Destructor
     */
    ~RadioButton ();

    /**
     * Redefined method for handling a mouseButton event
     * @param e A pointer to the event that occured
     */
    void handleEventMouseButton(pron::EventMouseButton *e) ;

    /** get the group of the radioButton
     * @return the group of the radioButton
     */
    RadioButtonGroup * getGroup();

    /**
     * set the group of the RadioButton
     * @param group The group to join
     */
    void setGroup(RadioButtonGroup * group);

    /**
     * removes the radioButton from its group
     */
    void resetGroup();

    /**
     * Join the group of another radioButton (only if the new group is not NULL)
     * @param button The Button whose group to join
     */
    void joinGroup (RadioButton * button);

    /**
     * Set the state of the radioButton
     * @param newState The new state to be set
     */
    void setState(bool newState);
  };


} // namespace sombrero


#endif //__RADIO_BUTTON_H__
