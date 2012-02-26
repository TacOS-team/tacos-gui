#ifndef __MOUSE_H__
#define __MOUSE_H__

#include <mouse_types.h>
#include <SDL/SDL.h>

ssize_t mouse_read(void *buf);
int mouse_ioctl(unsigned long request, void *data);

#endif //__MOUSE_H__
