MonAppli::monCallback(int val) {
  /*
   * Traitement du changement de valeur de la scrollbar avec Val la nouvelle valeur
   */
}

MonAppli::MonAppli() {
  this->scrollBar = new sombrero::ScrollBar();
  // Connection du signal au slot
  this->scrollBar->newValue.connect(this, &MonAppli::monCallback)
}

/*
 * Et voici comment la scrollbar emmet le signal :
 */
ScrollBar::fonction() {
  this->newValue(this->val);
}
