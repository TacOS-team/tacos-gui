MonAppli::monCallback(int val) {
  /*
   * Traitement du changement de valeur de la scrollbar (val est la nouvelle valeur)
   */
}

MonAppli::MonAppli() {
  this->scrollBar = new sombrero::ScrollBar();
  // Connection du signal au slot
  this->scrollBar->newValue.connect(this, &MonAppli::monCallback)
}

/*
 * Et voici comment la scrollbar emet le signal :
 */
class Scrollbar : public Widget {
  // ...

  /**
   * Signal envoye lorsque la valeur a change. La nouvelle valeur est passee en parametre.
   */
  signal1<int> newValue;
}

ScrollBar::fonction() {
  this->newValue(this->val);
}
