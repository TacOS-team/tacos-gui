#ifndef __IMAGES_H__
#define __IMAGES_H__

#define VAL_MAX 255

//#define DEBUG

/** 
 * Definition du type representant une image en
 * Image couleur
 * Les pixels sont lu de ligne en ligne 
 */
struct pixel{
  unsigned char b;
  unsigned char g;
  unsigned char r;
  unsigned char a;
};

struct image_info{
  char mode;
  int width;
  int height;
  int profondeur;
};

/**
 * Permet de lire une image PPM ASCII
 */
struct pixel * lireCoulImage(char * nom, struct image_info * image);

#endif//__IMAGES_H__
