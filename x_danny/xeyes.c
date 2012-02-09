#include <stdio.h>
#include <SDL/SDL.h>
#include <math.h>

#define LARG 640
#define HAUT 480


#define EYE_RADIUS 200
#define EYE_X (LARG/2 - EYE_RADIUS)
#define EYE_Y (HAUT/2 - EYE_RADIUS)

#define PUP_RADIUS 20


void gererEvent(SDL_Event event, int * continuer, SDL_Rect * coordPup) {
    switch (event.type) {
	case SDL_QUIT :
	    *continuer = 0;
	    break;
	case SDL_MOUSEMOTION:
	    coordPup->x = event.motion.x;
	    coordPup->y = event.motion.y;
	    break;
    }
}


void setCoordPup(SDL_Rect * coordPup, SDL_Rect coordMouse) {
    int eyeCentreX = EYE_X + EYE_RADIUS;
    int eyeCentreY = EYE_Y + EYE_RADIUS;

    float CM = sqrt( pow(coordMouse.x - eyeCentreX,2) + pow(coordMouse.y - eyeCentreY,2) );

    int pupCentreX = ((coordMouse.x - eyeCentreX) * (EYE_RADIUS - PUP_RADIUS) / CM) + eyeCentreX;

    int pupCentreY = ((coordMouse.y - eyeCentreY) * (EYE_RADIUS - PUP_RADIUS) / CM) + eyeCentreY;

    coordPup->x = pupCentreX - PUP_RADIUS;
    coordPup->y = pupCentreY - PUP_RADIUS;
}

int main () {

    int continuer = 1;

    SDL_Surface *ecran = NULL;
    SDL_Event event;
    SDL_Surface * pup = SDL_LoadBMP("./pup.bmp");
    SDL_Rect coordPup;
    SDL_Rect coordMouse;

    coordPup.x = EYE_X;
    coordPup.y = EYE_Y;

    SDL_Init(SDL_INIT_VIDEO);
    ecran = SDL_SetVideoMode(LARG,HAUT,32,SDL_HWSURFACE);
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0 ));

    while (continuer) {
	if (SDL_PollEvent(&event)) {
	    gererEvent(event,&continuer,&coordMouse);
	    setCoordPup(&coordPup,coordMouse);

	    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255 ));
	    SDL_BlitSurface(pup,NULL,ecran,&coordPup);

	    SDL_Flip(ecran);
	}
    }



    return 0;
}


