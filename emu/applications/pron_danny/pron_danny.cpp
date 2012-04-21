#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#include <pron_danny.h>

/**
 * @author John Carmack
 */
float Q_rsqrt( float number ){
    long i;
    float x2, y;
    const float threehalfs = 1.5F;
 
    x2 = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;  // evil floating point bit level hacking
    i  = 0x5f3759df - ( i >> 1 ); // what the fuck?
    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) ); // 1st iteration
    // y  = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration, this can be removed
 
    #ifndef Q3_VM
    #ifdef __linux__
      assert( !isnan(y) ); // bk010122 - FPE?
    #endif
    #endif
    return y;
}

float pow2( float number ){
  return number * number;
}

/**
 * PronDanny Constructors
 */

PronDanny::PronDanny(){

  this->eyeLeftX = X_DANNY_EYE_LEFT_X;
  this->eyeLeftY = X_DANNY_EYE_LEFT_Y;
  this->eyeLeftRadius = X_DANNY_EYE_LEFT_RADIUS;
  this->eyeLeftPupilRadius = X_DANNY_EYE_LEFT_PUPIL_RADIUS;

  this->eyeRightX = X_DANNY_EYE_RIGHT_X;
  this->eyeRightY = X_DANNY_EYE_RIGHT_Y;
  this->eyeRightRadius = X_DANNY_EYE_RIGHT_RADIUS;
  this->eyeRightPupilRadius = X_DANNY_EYE_RIGHT_PUPIL_RADIUS;

}

/**
 * Prondanny Overloaded Operators
 */
ostream& operator<< (ostream &out, PronDanny &pronDanny)
{
  out << "PronDanny instance : " << endl 
      << "eyeLeftX : " << pronDanny.eyeLeftX << "eyeLeftY : " << pronDanny.eyeLeftY << endl
      << "eyeLeftRadius : " << pronDanny.eyeLeftRadius << "eyeLeftPupilRadius : " << pronDanny.eyeLeftPupilRadius
      << "eyeRightX : " << pronDanny.eyeRightX << "eyeRightY : " << pronDanny.eyeRightY << endl
      << "eyeRightRadius : " << pronDanny.eyeRightRadius << "eyeRightPupilRadius : " << pronDanny.eyeRightPupilRadius;
  return out;
}

/**
 * PronDanny Methods
 */

void PronDanny::setFullScreen(bool fullscreen){
  this->fullscreen = fullscreen;
}

void PronDanny::setLolMode(bool lolMode){
  this->lolMode = lolMode;
}

void PronDanny::setMoveMode(bool moveMode){
  this->moveMode = moveMode;
}

/**
 * Main
 */
int main (int argc, char **argv){ 
  PronDanny danny;
  int c;
  while ((c = getopt (argc, argv, "flm")) != -1){
    switch (c){
    case 'f':
      danny.setFullScreen(true);
      break;
    case 'l':
      danny.setLolMode(true);
      break;
    case 'm':
      danny.setMoveMode(true);
      break;
    default:
      abort ();
    }
  } 
}
