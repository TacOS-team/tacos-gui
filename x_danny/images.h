#ifndef __IMAGES_H__
#define __IMAGES_H__

#define VAL_MAX 255

#define R 0
#define G 1
#define B 2

//#define DEBUG

/** 
 * Definition du type representant une image en
 * Image couleur
 * Les pixels sont lu de ligne en ligne 
 */
typedef struct pixel{
  unsigned char r;
  unsigned char g;
  unsigned char b;
} *coulIm;

typedef struct image_info{
  char mode;
  int width;
  int height;
  int profondeur;
} *image_info;

/**
 * Permet de lire une image PPM ASCII
 */
coulIm lireCoulImage(char *nom, image_info image);

#endif//__IMAGES_H__
