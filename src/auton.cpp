#include "vex.h"

void pre_auton(void) {
  vexcodeInit();
  Brain.Screen.printAt(300,300,"Hello!");
}

void autonomous(void) {

}
