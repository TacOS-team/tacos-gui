#ifndef __VGA_H__
#define __VGA_H__

#include <vga_types.h>

void vga_set_mode(enum vga_mode mode);
void vga_write_buf(char *buffer);
int vga_ioctl(unsigned long request, void *data);

#endif //__VGA_H__
