#ifndef __PRON_DANNY_H__
#define __PRON_DANNY_H__

#include <iostream>

using namespace std;

#define X_DANNY_EYE_LEFT_X 34
#define X_DANNY_EYE_LEFT_Y 64
#define X_DANNY_EYE_LEFT_RADIUS 20
#define X_DANNY_EYE_LEFT_PUPIL_RADIUS 10

#define X_DANNY_EYE_RIGHT_X 73
#define X_DANNY_EYE_RIGHT_Y 70
#define X_DANNY_EYE_RIGHT_RADIUS 20
#define X_DANNY_EYE_RIGHT_PUPIL_RADIUS 10

class PronDanny {
  
  friend ostream& operator<< (ostream &out, PronDanny &pronDanny);
  
 private:

  bool fullscreen;
  bool lolMode;
  bool moveMode;

  int eyeLeftX;
  int eyeLeftY;
  int eyeLeftRadius;
  int eyeLeftPupilRadius;

  int eyeRightX;
  int eyeRightY;
  int eyeRightRadius;
  int eyeRightPupilRadius;

 public:
  
  PronDanny();
  //~PronDanny();

  void setFullScreen(bool fullScreen);
  void setLolMode(bool lolMode);
  void setMoveMode(bool moveMode);

};

#endif//__PRON_DANNY_H__
