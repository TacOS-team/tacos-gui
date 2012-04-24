#include "sombrero.h"
#include "stdlib.h"
#include "stdio.h"
#include "pronlib.h"
#include "application.h"

namespace sombrero {

/**
 * Prints the sombrero logo
 */
void sombrero_ascii_art() {
  printf("\n"
    " (       )    *         (       (       )   " "\n"
    " )\\ ) ( /(  (  `     (  )\\ )    )\\ ) ( /(   " "\n"
    "(()/( )\\()) )\\))(  ( )\\(()/((  (()/( )\\())  " "\n"
    " /(_)|(_)\\ ((_)()\\ )((_)/(_))\\  /(_)|(_)\\   " "\n"
    "(_))   ((_)(_()((_|(_)_(_))((_)(_))   ((_)  " "\n"
    "/ __| / _ \\|  \\/  || _ ) _ \\ __| _ \\ / _ \\  " "\n"
    "\\__ \\| (_) | |\\/| || _ \\   / _||   /| (_) | " "\n"
    "|___/ \\___/|_|  |_||___/_|_\\___|_|_\\ \\___/  " "\n"
  );
}

} // namespace sombrero
