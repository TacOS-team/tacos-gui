#include <cstdlib>
#include <cstdio>
#include "pronlib.h"
#include <fcntl.h>
#include <unistd.h>

#define PI 3.14159f

#define LARGEUR 320
#define HAUTEUR 200

#define FRICTION 0.9f

float cube[][3]= {
	{  1.0f,  1.0f, -1.0f },
	{  1.0f, -1.0f, -1.0f },
	{ -1.0f, -1.0f, -1.0f },
	{ -1.0f,  1.0f, -1.0f },
	{  1.0f,  1.0f,  1.0f },
	{  1.0f, -1.0f,  1.0f },
	{ -1.0f, -1.0f,  1.0f },
	{ -1.0f,  1.0f,  1.0f }
};

int lignes[][2] = {
	{0, 1}, // face du haut
	{1, 2},
	{2, 3},
	{3, 0},
	{4, 5}, // face du bas
	{5, 6},
	{6, 7},
	{7, 4},
	{0, 4}, // Arrêtes des cotés
	{1, 5},
	{2, 6},
	{3, 7}
};

float my_cos(float angle) {
	float angle_radians = angle*(2*PI)/360;
	float result;
	asm("fcos" : "=t" (result) : "0" (angle_radians));
	return result;
}

float my_sin(float angle) {
	float angle_radians = angle*(2*PI)/360;
	float result;
	asm("fsin" : "=t" (result) : "0" (angle_radians));
	return result;
}

void rotate_point(float point[3], float theta, float phi) {
	float temp[3];
	
	float cos_theta = 	my_cos(theta);
	float sin_theta = 	my_sin(theta);
	
	float cos_phi = 	my_cos(phi);
	float sin_phi = 	my_sin(phi);
	
	temp[0] = point[0];
	temp[1] = point[1]*cos_theta - point[2]*sin_theta;
	temp[2] = point[1]*sin_theta + point[2]*cos_theta;
	
	point[0] = temp[0]*cos_phi - temp[2]*sin_phi;
	point[1] = temp[1];
	point[2] = temp[0]*sin_phi + temp[2]*cos_phi;
}

void draw_cube(Display *d, Window w, GC gc) {
	int ligne;

	//On efface la fenêtre
	pronClearWindow(d, w);
	
	for (ligne = 0; ligne < 12; ligne++) {
		pronDrawLine(d, w, gc, 
			(int)(cube[lignes[ligne][0]][0]*50+150), 
			(int)(cube[lignes[ligne][0]][1]*50+100), 
			(int)(cube[lignes[ligne][1]][0]*50+150), 
			(int)(cube[lignes[ligne][1]][1]*50+100));
	}
}

int main(int argc, char **argv){
	int i = 0;
	int x, y;
	int prevx,prevy;
	bool buttonPressed;
	
	float theta = 0.0f;
	float phi = 0.0f;
	// On se connecte a PRON
	Display* d = pronConnect();
	if ( d==NULL ) {
		return 1;
	}
	// On cree la fenêtre
	Window w = pronCreateWindow(d, d->rootWindow, 0, 0, LARGEUR, HAUTEUR);
	// On cree un contexte graphique
	GC gc = pronCreateGC(d);
	// On mappe la fenêtre
	pronMapWindow(d, w);
	// S'abonne aux évènements
	pronSelectInput(d, w, PRON_EVENTMASK(EV_POINTER_MOVED) | PRON_EVENTMASK(EV_MOUSE_BUTTON));
	// On cree un énènement
	PronEvent *e = getPronEvent();
	while (1) {	
		//On récupère un évènement
    if (!pronNextEvent(d, e)) {
      fprintf(stderr, "pron has closed the connection.\n");
      exit(1);
    }

    printf("Type e %d\n", (e->type & ~(11 << 14)) );

		switch (e->type) {
      case EV_POINTER_MOVED: {
        EventPointerMoved *pointerMoved = (EventPointerMoved*) e;
      	x = pointerMoved->x;
      	y = pointerMoved->y;
        break;
      }
      case EV_MOUSE_BUTTON : {
        EventMouseButton *mouseButton = (EventMouseButton*) e;
      	buttonPressed = mouseButton->b1;
				printf("Bouton %d %d %d %d %d %d\n", mouseButton->b1, mouseButton->b2, mouseButton->b3, mouseButton->b4, mouseButton->b5, mouseButton->b6);
        break;
      }
      default:
        break;
    }
		
		if (buttonPressed) {// Bouton 1 appuyé
			phi = (float)(x-prevx) * FRICTION;
			theta = (float)(y-prevy) * FRICTION;
		}	else {
			printf("appuyé\n");
			phi = FRICTION * phi;
			theta = FRICTION * theta;
		}
		for (i = 0; i < 8; i++) {
			rotate_point(cube[i],theta,phi);
		}
		
		draw_cube(d, w, gc);
		
		prevx = x;
		prevy = y;
		
		usleep(10000);
	}

}