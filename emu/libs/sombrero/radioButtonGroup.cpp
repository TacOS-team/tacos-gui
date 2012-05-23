#include "radioButtonGroup.h"

namespace sombrero {

  RadioButtonGroup::RadioButtonGroup() {
    this->currentCheckedButton = -1;
  }

  RadioButtonGroup::~RadioButtonGroup() {
  }

  void RadioButtonGroup::add (RadioButton * radioButton) {
    if (radioButton != NULL) {
      radioButtons.push_back(radioButton);
      radioButton->setGroup(this);
    }
  }

  void RadioButtonGroup::remove (RadioButton * radioButton) {
    bool found = false;
    int i = 0;
    while (!found && i < (int)this->radioButtons.size()) {
      if (radioButtons[i] == radioButton) {
        found = true;
        this->radioButtons[i] = this->radioButtons[this->radioButtons.size() - 1];
        this->radioButtons.pop_back();
      }
      i++;
    }
  }


  void RadioButtonGroup::setNewCheckedButton (RadioButton * newCheckedButton) {
    int i = 0;
    bool found = false;

    if (this->currentCheckedButton != -1) {
      this->radioButtons[this->currentCheckedButton]->setState(false);
      this->radioButtons[this->currentCheckedButton]->draw();
    }

    while (!found && i < (int)radioButtons.size()) {
      if (radioButtons[i] == newCheckedButton) {
        found = true;
        this->currentCheckedButton = i;
      }
      i++;
    }
  }

  std::vector<RadioButton *> RadioButtonGroup::getRadioButtons() {
    return this->radioButtons;
  }

} // namespace sombrero
