#ifndef __MOUSE_H__
#define __MOUSE_H__

#define PRON_MOUSE_POINTER_WIDTH 6
#define PRON_MOUSE_POINTER_HEIGHT 6

class Mouse {
private:
  // Pointer backup position
  int pointerBackupX;
  int pointerBackupY;
  bool pointerBackuped;
  // Pointer backup
  char *pointerBackup;
  // True if we have to restore pointer background
  bool pointerBackupRestore;

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
};

#endif
