#include "vex.h"
#include "odom.h"

void autonomous(void) {
  positiontrackingtask = task( positionTrack);

  turnSlide(12, 12, 90);
  turnSlide(0, 0, 0);
}

