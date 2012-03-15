#ifndef __KBD_H__
#define __KBD_H__

#include <kbd_types.h>
#include <SDL/SDL.h>

ssize_t kbd_read(void *buf);
int kbd_ioctl(unsigned long request, void *data);

#endif //__KBD_H__
