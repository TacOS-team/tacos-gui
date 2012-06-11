MonAppli::monCallback() {
  /*
   * Traitement lie au clic sur monBouton
   */
}

MonAppli::MonAppli() {
  /* Code */
  this->monBouton = new sombrero:Button("Valider");
  this->monBouton->clicked.connect(this, &MonAppli::monCallback)
  /* Code */
}
