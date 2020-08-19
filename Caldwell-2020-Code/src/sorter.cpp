#include "vex.h"

task ballsortertask = task(ballSort);

bool reject = false;
vision::signature unwantedColor;

vex::vision::signature getBall() { //gets whether there is a ball in front of the sensor, and what color
  int largestRed = 0;
  int largestBlue = 0;
  int width,height,size;
  vs.takeSnapshot(RED_BALL);
  if (vs.largestObject.exists) {
    width = vs.largestObject.width;
    height = vs.largestObject.height;
    size = width*height;
    if (size>600) {largestRed=width*height;}
  }
  vs.takeSnapshot(BLUE_BALL);
  if (vs.largestObject.exists) {
    width = vs.largestObject.width;
    height = vs.largestObject.height;
    size=width*height;
    if (size>600) {largestBlue=width*height;}
  }
  if (largestBlue>largestRed) {return BLUE_BALL;}
  else if (largestRed>largestBlue) {return RED_BALL;}
  else {return NOTHING;}
}

int ballSort() { //callback for the ball sorting task
  //TODO: write this
  return 0;
}