void Container::setParent(Widget *widget) {
  Widget::setParent(widget);
  vector<Widget*> children = this->getChildren();
  for (vector<Widget*>::iterator it = children.begin(); it != children.end(); ++it) {
    (*it)->setParent(this);
  }
}
