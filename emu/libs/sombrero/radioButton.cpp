#include "radioButton.h"

namespace sombrero {

RadioButton::RadioButton() : Checkbox() {
  this->group = NULL;
}

RadioButton::RadioButton (RadioButtonGroup *group) : Checkbox () {
  this->setGroup(group);
}

RadioButton::~RadioButton() {
  this->resetGroup();
}

RadioButtonGroup * RadioButton::getGroup() {
  return this->group;
}

void RadioButton::setGroup (RadioButtonGroup * group, bool addToGroup) {
  this->group = group;
  if (addToGroup && group != NULL) {
    group->add(this);
  }
    
}

void RadioButton::resetGroup () {
  if (this->group != NULL) {
    this->group->remove(this);
    this->group = NULL;
  }
}

void RadioButton::joinGroup (RadioButton * radioButton) {
  RadioButtonGroup * newGroup = radioButton->getGroup();
  if (newGroup!=NULL) {
    radioButton->getGroup()->add(this);
  } 
}

void RadioButton::handleMouseDown(MouseButton button, int x __attribute__((unused)), int y __attribute__((unused))) {
  if (button == leftButton) {
    this->clicked();
    if (this->state == false) {
      if (this->group != NULL) {
        this->group->setNewCheckedButton(this);
      }
      this->state = true;
      this->draw();
    }
  }
}

void RadioButton::setState(bool newState) {
  this->state = newState;
}

} // namespace sombrero
