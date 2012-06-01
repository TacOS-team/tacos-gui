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
  
  protected:
    void handleMouseDown(MouseButton button, int x, int y);

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

    /** get the group of the radioButton
     * @return the group of the radioButton
     */
    RadioButtonGroup * getGroup();

    /**
     * set the group of the RadioButton
     * @param group The group to join
     * @param setGroup Indiquates whether the group->add(this) has to be called
     */
    void setGroup(RadioButtonGroup * group, bool addToGroup = true);

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
