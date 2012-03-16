#ifndef __PRONLIB_STRUCTS_H__
#define __PRONLIB_STRUCTS_H__

#include <vesa_types.h>

/**
 * Structure for pron window attributes
 */
struct PronWindowAttributes {
  unsigned short x;
  unsigned short y;
  unsigned short width;
  unsigned short height;
  color_t        bgColor;
};
/**
 * Structure for pron Image
 */
struct PronImage {
	int width, height;// Size of the image
	int format;// XYBitmap, XYPixmap, ZPixmap
	char *data;// Pointer to image data
	int depth;// Depth of the image 24, 16, 8 bits
};

#endif // __PRONLIB_STRUCTS_H__
