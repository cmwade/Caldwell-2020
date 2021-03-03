/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/


#include "vex.h"

void pre_auton(void);
void autonomous(void);
void usercontrol(void);

using namespace vex;

competition Competition;

int main() {
  Competition.drivercontrol(usercontrol);
  Competition.autonomous(autonomous);
  pre_auton();

  while (true) {
    wait(20, msec);
  }
}
