#ifndef __VESA_H__
#define __VESA_H__

#include <vesa_types.h>

void vesa_set_mode(struct vesa_setmode_req* mode);
void vesa_flush();
int vesa_ioctl(unsigned long request, void *data);

#endif //__VESA_H__
