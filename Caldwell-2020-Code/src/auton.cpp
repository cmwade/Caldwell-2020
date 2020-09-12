#include "vex.h"

void Test( void ) {
  driveReset(0, 0, 0);
  turnSlide(0, 24, -90);
  turnSlide(24, 24, 180);
  turnSlide(24, 0, 90);
  turnSlide(0, 0, 0);
  turnSlide(0, 24, -90);
  turnSlide(24, 24, 180);
  turnSlide(24, 0, 90);
  turnSlide(0, 0, 0);
  turnSlide(0, 24, -90);
  turnSlide(24, 24, 180);
  turnSlide(24, 0, 90);
  turnSlide(0, 0, 0);
}

void autonomous(void) {
  task positiontrackingtask = task(positionTrack);
  Test(); //Results from Auton Selector Determine the Auto Chosen
}
