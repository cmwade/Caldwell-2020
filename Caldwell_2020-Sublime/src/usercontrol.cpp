#include "vex.h"

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

        if(con.ButtonR1.pressing()){
          IntakeR.spin(directionType::fwd, 100, velocityUnits::pct);
          IntakeL.spin(directionType::fwd, 100, velocityUnits::pct);
        } else if (con.ButtonR2.pressing()) {
          IntakeR.spin(directionType::rev, 100, velocityUnits::pct);
          IntakeL.spin(directionType::rev, 100, velocityUnits::pct);
        } else {
          IntakeR.stop(brakeType::hold);
          IntakeL.stop(brakeType::hold);
        }
        wait(20, msec);
  }
}
