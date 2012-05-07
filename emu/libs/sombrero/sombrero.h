#ifndef __SOMBRERO_H__
#define __SOMBRERO_H__
/**
 * @file sombrero.h
 * Main sombrero header
 */

#include "widget.h"
#include "container.h"
#include "window.h"
#include "application.h"
#include "bin.h"
#include "canvas.h"
#include "button.h"
#include "timer.h"
#include "textbox.h"
#include "checkbox.h"
#include "label.h"

/**
 * Sombrero library namespace
 */
namespace sombrero {

/**
 * Prints the sombrero ascii art welcome message
 */
void sombrero_ascii_art();

/**
 * Initializes sombrero, connecting to pron getting a display.
 * Exits the program if it fails to connect to pron.
 */
void init();

enum PositionType {
  POS_LEFT,
  POS_RIGHT,
  POS_TOP,
  POS_BOTTOM,
};

} // namespace sombrero

#endif//__SOMBRERO_H__
