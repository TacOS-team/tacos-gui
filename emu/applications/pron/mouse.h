#ifndef __MOUSE_H__
#define __MOUSE_H__

/**
 * @file mouse.h
 * Mouse class definition.
 */

#include <mouse_types.h>

#define PRON_MOUSE_POINTER_WIDTH 6 /**< Width of the pointer */
#define PRON_MOUSE_POINTER_HEIGHT 6 /**< Height of the pointer */

/**
 * Mouse.
 */
class Mouse {
 private: 
  static Mouse *instance; /**< Unique Mouse instance (singleton) */
  int fd; /**< File descriptor used to communicate with the mouse driver */
  
  int mouseX; /**< Mouse absolute x position */
  int mouseY; /**< Mouse absolute y position */
  bool mouseB1; /**< State of button 1 (left button) */
  bool mouseB2; /**< State of button 2 (right button) */
  bool mouseB3; /**< State of button 3 */
  bool mouseB4; /**< State of button 4 */
  bool mouseB5; /**< State of button 5 */
  bool mouseB6; /**< State of button 6 */

  unsigned long lastMouseEvent; /**< Time of the last mouse event */
  int lastSentX; /**< Last sent mouse x position */
  int lastSentY; /**< Last sent mouse y position */

  bool pointerBackuped; /**< True if the area under the pointer has been backuped */
  char *pointerBackup; /**< Backup of the area under the pointer */
  int pointerBackupX; /**< Top-left corner x-coordinate of the backuped area */
  int pointerBackupY; /**< Top-left corrner y-coordinate of the backuped area */
  
  /**
   * Constructor.
   */
  Mouse();

 public:
  /**
   * Returns the unique Mouse instance (singleton). Creates it if necessary.
   * @return the unique Mouse instance (singleton)
   */
  static Mouse* getInstance();

  /**
   * Reads events from the mouse and handles them.
   */
  void checkEvents();

  /**
   * Updates the window the mouse is currently in.
   */
  void updateMouseWin();

  /**
   * Updates the window that currently has the focus.
   */
  void updateFocusWin();

  /**
   * Restores the area under the mouse pointer.
   */
  void restorePointerBackground();

  /**
   * Draws the mouse pointer. The area under the pointer is backuped.
   */
  void drawPointer();

  /**
   * Handles mouse motion events.
   */
  void handleMotion(mousestate_t *state);

  /**
   * Handles mouse button events.
   */
  void handleButton(mousestate_t *state);

  /**
   * Returns the x position of the mouse.
   * @return the x position of the mouse
   */
  int getMouseX();

  /**
   * Returns the y position of the mouse.
   * @return the y position of the mouse
   */
  int getMouseY();

  /**
   * Returns the state of button 1 (left button).
   * @return the state of button 1 (left button)
   */
  bool getMouseB1();

  /**
   * Returns the state of button 2 (right button).
   * @return the state of button 2 (right button)
   */
  bool getMouseB2();

  /**
   * Returns the state of button 3.
   * @return the state of button 3
   */
  bool getMouseB3();

  /**
   * Returns the state of button 4.
   * @return the state of button 4
   */
  bool getMouseB4();

  /**
   * Returns the state of button 5.
   * @return the state of button 5
   */
  bool getMouseB5();

  /**
   * Returns the state of button 6.
   * @return the state of button 6
   */
  bool getMouseB6();
};

#endif
