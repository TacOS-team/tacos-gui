#ifndef __MOUSE_H__
#define __MOUSE_H__

#include <mouse_types.h>

#define PRON_MOUSE_POINTER_WIDTH 6
#define PRON_MOUSE_POINTER_HEIGHT 6

class Mouse {
private:
  
  static Mouse *instance;

  // Pointer backup position
  int pointerBackupX;
  int pointerBackupY;
  bool pointerBackuped;
  // Pointer backup
  char *pointerBackup;
  // True if we have to restore pointer background
  bool pointerBackupRestore;
  
  // Passage a un constructeur privé
  Mouse();

  int mouseX; // Mouse state
  int mouseY;
  bool mouseB1;
  bool mouseB2;
  bool mouseB3;
  bool mouseB4;
  bool mouseB5;
  bool mouseB6;

public:
  int fd;

  void checkEvents();
  /**
   * Update mouseWin
   */
  void updateMouseWin();
  /**
   * Restore the mouse pointer
   */
  void restorePointerBackground();
  /**
   * Backup and draw mouse pointer
   */
  void drawPointer();
  /**
   * Handle mouse motion events
   */
  void handleMotion(mousestate_t *state);
  /**
   * Handle button motion events
   */
  void handleButton(mousestate_t *state);
  /**
   * Getters and setters
   */
  int getPointerBackupX();
  void setPointerBackupX(int pointerBackupX);
  int getPointerBackupY();
  void setPointerBackupY(int pointerBackupY);
  int getPointerBackupWidth();
  void setPointerBackupWidth(int pointerBackupWidth);
  int getPointerBackupHeight();
  void setPointerBackupHeight(int pointerBackupHeight);
  char* getPointerBackup();  
  void setMouseX(int mouseX);
  int getMouseX();
  void setMouseY(int mouseY);
  int getMouseY();
  void setMouseB1(bool mouseB1);
  bool getMouseB1();
  void setMouseB2(bool mouseB2);
  bool getMouseB2();
  void setMouseB3(bool mouseB3);
  bool getMouseB3();
  void setMouseB4(bool mouseB4);
  bool getMouseB4();
  void setMouseB5(bool mouseB5);
  bool getMouseB5();
  void setMouseB6(bool mouseB6);
  bool getMouseB6();

  static Mouse* getInstance();
};

#endif
