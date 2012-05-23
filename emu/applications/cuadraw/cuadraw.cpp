#include <stdio.h>
#include <stdlib.h>
#include <sombrero.h>
#include "cuadraw.h"

Cuadraw::Cuadraw(int argc, char **argv) 
    : fileIn(NULL), fileOut(NULL) {
  this->init(argc, argv);
  this->initSombrero();
}

Cuadraw::~Cuadraw() {
  delete this->w;
  if (this->fileIn != NULL) {
    fclose(this->fileIn);
  }
  if (this->fileOut != NULL) {
    fclose(this->fileOut);
  }
}

void Cuadraw::init(int argc, char **argv) {
  int opt;
  while ((opt = getopt(argc, argv, "i:o:")) != -1) {
    switch (opt) {
      case 'i':
        this->fileIn = fopen(optarg, "r");
        if (this->fileIn == NULL) {
          fprintf(stderr, "File given in input not found, starting from sratch.\n");
        }
        break;
      case 'o':
        this->fileOut = fopen(optarg, "w");
        if (this->fileOut == NULL) {
          fprintf(stderr, "Failed to create an output file, aborting.\n");
        }
        break;
      default: /* '?' */
        fprintf(stderr, "Usage: %s [-i fileIn] [-o fileOut]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }
  // Traitement des cas ou on a pas de fichier de sortie
  if (this->fileOut == NULL) {
    char *tempNam = tempnam(TEMP_NAM_FOLDER, TEMP_NAM_PREFIX);
    this->fileOut = fopen(tempNam, "w");
    if (this->fileOut == NULL) {
      fprintf(stderr, "Failed to create the default output file %s\n", tempNam);
    } else {
      printf("Writing in a default output file %s\n", tempNam);
    }
    free(tempNam);
  }
}

void Cuadraw::initSombrero() {
  this->w = new sombrero::Window(0, 0, 500, 500);
  //this->g = new sombrero::Grid(w);
  //this->bp = new sombrero::Button("Pixel");
  //this->bc = new sombrero::Button("Cirle");
  //this->c = new sombrero::Canvas(g, 300, 300);
  sombrero::Application::getInstance()->sombrerun();
}

int main (int argc, char **argv) {
  Cuadraw c(argc, argv);
  return 0;
}
