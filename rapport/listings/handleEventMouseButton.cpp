void Widget::handleEventMouseButton(pron::EventMouseButton *e) {
  // Si le bouton du milieu a change d'etat
  if (this->oldButtonsState.b2 != e->b2) {
    if (e->b2) { // S'il est enfonce
      this->handleMouseDown(middleButton, e->x, e->y);
    } else { // S'il a ete relache
      this->handleMouseReleased(middleButton, e->x, e->y);
    }
  }
  
  // Meme chose pour le bouton droit...
  
  // Si le bouton de gauche a change d'etat
  if (this->oldButtonsState.b1 != e->b1) {
    if (e->b1) { // S'il est enfonce
      // Calcul du temps entre le dernier clic et ce clic
      struct timeval tv;
      gettimeofday(&tv, NULL);
      // Si temps inferieur a doubleClickDelay millisecondes
      if (((tv.tv_sec - lastClick.tv_sec) * 1000000
          + tv.tv_usec - lastClick.tv_usec) < doubleClickDelay * 1000) {
        // On appelle la fonction virtuelle pour un double clic
        this->handleDoubleClick(e->x, e->y);
        // On reinitialise la date du dernier clic
        lastClick.tv_sec = 0;
      } else {
        // Sinon on met a jour la date de dernier clic
        lastClick = tv;
      }
      // Dans tous les cas, on appelle le handler de mouseDown
      this->handleMouseDown(leftButton, e->x, e->y);
    } else {
      // Si pas enfonce il est relache
      this->handleMouseReleased(leftButton, e->x, e->y);
      // Si en plus la souris est au dessus du widget a ce moment-la, on notifie d'un clic
      if (e->x >= 0 && e->y >= 0 &&
          e->x < this->getWidth() && e->y < this->getHeight()) {
        this->handleClick(e->x, e->y);
      }
    }
  }
  this->oldButtonsState = *e;
}
