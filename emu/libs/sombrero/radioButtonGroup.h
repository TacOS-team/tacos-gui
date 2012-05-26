#ifndef __RADIO_BUTTON_GROUP__
#define __RADIO_BUTTON_GROUP__

/**
 * @file radioButtonGroup.h
 * RadioButtonGroup class definition
 */

#include "widget.h"
#include "radioButton.h"


namespace sombrero {
class RadioButton;

class RadioButtonGroup {
  private :
    std::vector<RadioButton *> radioButtons; /**< The vector of radioButtons that are part of the group */
    int currentCheckedButton; /**< The index in radioButtons of the currently checked button */

  public : 
    /**
     * RadioButtonGroup constructor
     */
    RadioButtonGroup ();

    /**
     * RadioButtonGroup destructor
     */
    ~RadioButtonGroup ();

    /**
     * add a radio button to the group
     * @param radioButton The button to add
     */
    void add (RadioButton * radioButton);

    /**
     * removes a radio button from the group
     * @param radioButton The button to remove
     */
    void remove (RadioButton * radioButton);

    /**
     * Set the new checked radioButton
     * @param newCheckedButton The new checked radioButton
     */
    void setNewCheckedButton (RadioButton * newCheckedButton);
    
    /**
     * Get the currently checked radioButton
     * @return The currently checked button. NULL if there is none
     */
    RadioButton * getCheckedButton ();

    /**
     * returns the radioButtons that are part of the group
     * @return the vector of radioButtons
     */
    std::vector<RadioButton *> getRadioButtons();
};

}// namespace sombrero


#endif // __RADIO_BUTTON_GROUP__
