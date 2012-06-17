void Widget::update() {
  // S'il n'est pas deja en cours de mise a jour
  // (pour eviter les boucles de mises a jour)
  if (!this->isUpdating) {
    this->isUpdating = true;
    // Si le parent n'a pas deja unmap la fenetre, on l'unmap
    if (this->getParent() && !this->getParent()->isUpdating) {
      this->setVisible(false);
    }
    // On execute la mise a jour
    this->execUpdate();
    // On remap si besoin
    if (this->getParent() && !this->getParent()->isUpdating) {
      this->setVisible(true);
    }
    this->isUpdating = false;
  }
}
