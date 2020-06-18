#include "vex.h"
#include "odom.h"

void usercontrol(void) {
  float throttle;
  float strafe;
  float turn;
  positiontrackingtask = task( positionTrack );
  while(1) {
        throttle = con.Axis3.value();
        strafe = con.Axis4.value();
        turn = con.Axis1.value();

        LF.spin(directionType::fwd, (throttle+strafe+turn), velocityUnits::pct);
        LB.spin(directionType::fwd, (throttle-strafe+turn), velocityUnits::pct);
        RB.spin(directionType::fwd, (throttle+strafe-turn), velocityUnits::pct);
        RF.spin(directionType::fwd, (throttle-strafe-turn), velocityUnits::pct);
        wait(20, msec);
  }
}
