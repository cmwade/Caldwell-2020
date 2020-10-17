#include "vex.h"

#define NO_BALL 0
#define BLUE 1
#define RED 2

int ballstatus = NO_BALL;

int unwantedColor;

int getBall() { //gets whether there is a ball in front of the sensor, and what color
  int largestRed = 0;
  int largestBlue = 0;
  int minimumArea = 30000;
  int width,height,size;
  vs.takeSnapshot(RED_BALL);
  if (vs.largestObject.exists) {
    width = vs.largestObject.width;
    height = vs.largestObject.height;
    size = width*height;
    if (size>minimumArea) {largestRed=width*height;}
  }
  vs.takeSnapshot(BLUE_BALL);
  if (vs.largestObject.exists) {
    width = vs.largestObject.width;
    height = vs.largestObject.height;
    size=width*height;
    if (size>minimumArea) {largestBlue=width*height;}
  }
  if (largestBlue>largestRed) {return BLUE;}
  else if (largestRed>largestBlue) {return RED;}
  else {return NO_BALL;}
}

int ballSort() { //callback for the ball sorting task
  while(true) {
    ballstatus = getBall();
    task::sleep(25);
  }
  return 0;
}