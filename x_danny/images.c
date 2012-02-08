#include "images.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>

// mode = "r" ou "w"
void ouvrirFichier (char *nom, FILE **desc, char *mode){
  *desc = fopen (nom, mode);
  if (*desc == NULL){
    fprintf (stderr,
	     "\"%s\": nom de fichier incorrect", 
	     nom);
    if (mode [0] == 'w')
      fprintf (stderr, " ou ouverture en ecriture impossible.");
    exit (EXIT_FAILURE);
  }
}


// Lecture et verification que l'en-tete est correcte et correspond 
// bien a notre mode (P2 = ndg ASCII, P3 = rgb ASCII)
void lireEnTete (FILE *descFic, struct image_info * image){
  char c;
  int v;
  
  fscanf (descFic, "%c", &c);
  // Lecture du mode
  if (c != 'P'){
    fprintf (stderr, "Mode non defini (%c*).\n", c);
    exit (EXIT_FAILURE);
    }
  fscanf (descFic, "%c", &c);
  if (c != image->mode){
    fprintf (stderr, "%c est necessaire pour une image ASCII.\n", image->mode);
    exit (EXIT_FAILURE);
  }
  // Lecture des dimensions
  fscanf (descFic, "%d", &v);
  image->width = v;
  fscanf (descFic, "%d", &v);
  image->height = v;
  // Et lecture de la valeur maximale d'une intensite
  fscanf (descFic, "%d", &v);
  if (v != VAL_MAX){
      fprintf (stderr, "L'intensite maximale doit etre de %d.\n", VAL_MAX);
      exit (EXIT_FAILURE);
  }
  image->profondeur = v;  
}

struct pixel * lireCoulImage (char *nom, struct image_info * image){
  FILE *descFic = NULL;
  int c, x, y;
  
  ouvrirFichier (nom, &descFic, "r");
  
  //récupération des infos de l'image
  memset(image, 0, sizeof(struct image_info));
  image->mode = '3';
  lireEnTete (descFic, image);

#ifdef DEBUG
  printf("format de l'image : largeur %d hauteur %d profondeur %d\n",image->width, image->height, image->profondeur);
#endif

  //création de l'espace mémoire de l'image
  struct pixel * im = malloc(image->width * image->height * sizeof(struct pixel));
  
  for (y=0; y < image->height; y++){  
    for (x=0; x < image->width; x++){
      fscanf (descFic, "%d", &c);
      im [x + y * image->width].r = c; 
      fscanf (descFic, "%d", &c);
      im [x + y * image->width].g = c; 
      fscanf (descFic, "%d", &c);
      im [x + y * image->width].b = c; //mode rgbfounke
#ifdef DEBUG
      printf("pixel {%d,%d,%d}\n",im [x * y].r,im [x * y].g,im [x * y].b);
#endif
    }
  }
  
  fclose (descFic);

  return im;
}
