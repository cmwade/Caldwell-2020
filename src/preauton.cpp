#include "vex.h"

void pre_auton(void) {
  vexcodeInit();
  Brain.Screen.printAt(100,100,"Hello!");
}
