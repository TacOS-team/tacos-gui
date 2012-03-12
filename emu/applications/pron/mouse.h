#ifndef __MOUSE_H__
#define __MOUSE_H__

class Mouse {
public:
  int fd;

  Mouse();

  void checkEvents();
  /**
   * Update mouseWin
   */
  void updateMouseWin();
  /**
   * Draws mouse pointer
   */
  void drawPointer();
};

#endif
