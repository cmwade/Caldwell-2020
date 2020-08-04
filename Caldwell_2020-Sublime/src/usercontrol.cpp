#include "vex.h"

void usercontrol(void) {
  float throttle;
  float strafe;
  float turn;
  positiontrackingtask = task( positionTrack );
  while(1) {
        
    //Drivebase Code

    throttle = con.Axis3.value();
    strafe = con.Axis4.value();
    turn = con.Axis1.value();

    LF.spin(directionType::fwd, (throttle+strafe+turn), velocityUnits::pct);
    LB.spin(directionType::fwd, (throttle-strafe+turn), velocityUnits::pct);
    RB.spin(directionType::fwd, (throttle+strafe-turn), velocityUnits::pct);
    RF.spin(directionType::fwd, (throttle-strafe-turn), velocityUnits::pct);

    //Intake and Roller Code

    if (con.ButtonR1.pressing()) { //Spinning Everything In
      if (con.ButtonL2.pressing()){
        IntakeR.spin(directionType::fwd, 100, velocityUnits::pct);
        IntakeL.spin(directionType::fwd, 100, velocityUnits::pct);
        RollerMain.spin(directionType::fwd, 100, velocityUnits::pct);
        RollerBack.spin(directionType::rev, 100, velocityUnits::pct);
      } else { //Spinning Everything Out the Back Roller
        IntakeR.spin(directionType::fwd, 100, velocityUnits::pct);
        IntakeL.spin(directionType::fwd, 100, velocityUnits::pct);
        RollerMain.spin(directionType::fwd, 100, velocityUnits::pct);
        RollerBack.spin(directionType::fwd, 100, velocityUnits::pct);
      }
    } else if (con.ButtonR2.pressing()) { //Spinning Everything Out the Bottom
        if (con.ButtonL2.pressing()){
        IntakeR.spin(directionType::rev, 100, velocityUnits::pct);
        IntakeL.spin(directionType::rev, 100, velocityUnits::pct);
        RollerMain.stop(brakeType::coast);
        RollerBack.stop(brakeType::coast);
      } else { //Opening the Intakes
        IntakeR.spin(directionType::rev, 100, velocityUnits::pct);
        IntakeL.spin(directionType::rev, 100, velocityUnits::pct);
        RollerMain.spin(directionType::rev, 100, velocityUnits::pct);
        RollerBack.spin(directionType::rev, 100, velocityUnits::pct);
      }
    } else if (con.ButtonL1.pressing()) { //Spinning Rollers Down
      if (con.ButtonL2.pressing()){
        IntakeR.stop(brakeType::hold);
        IntakeL.stop(brakeType::hold);
        RollerMain.spin(directionType::rev, 100, velocityUnits::pct);
        RollerBack.spin(directionType::rev, 100, velocityUnits::pct);
      } else { //Spinning Intakes and Main Roller In
        IntakeR.spin(directionType::fwd, 100, velocityUnits::pct);
        IntakeL.spin(directionType::fwd, 100, velocityUnits::pct);
        RollerMain.spin(directionType::fwd, 100, velocityUnits::pct);
        RollerBack.stop(brakeType::coast);
      }
    } else {
      IntakeR.stop(brakeType::hold);
      IntakeL.stop(brakeType::hold);
      RollerMain.stop(brakeType::coast);
      RollerBack.stop(brakeType::coast);
    }
  }
}
