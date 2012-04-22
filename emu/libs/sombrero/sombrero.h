#ifndef __SOMBRERO_H__
#define __SOMBRERO_H__
/**
 * @file sombrero.h
 * Main sombrero header
 */

#include "application.h"
#include "widget.h"
#include "window.h"

/**
 * Sombrero library namespace
 */
namespace sombrero {

/**
 * Initializes sombrero, connecting to pron getting a display.
 * Exits the program if it fails to connect to pron.
 */
void init();

} // namespace sombrero

#endif//__SOMBRERO_H__
