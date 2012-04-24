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

} // namespace sombrero

#endif//__SOMBRERO_H__
