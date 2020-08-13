#include "vex.h"

void stopIntakes(void) {
  IntakeL.stop(brakeType::hold);
  IntakeR.stop(brakeType::hold);
}

void intake(void) { //intake in
  IntakeR.spin(directionType::fwd, 100, velocityUnits::pct);
  IntakeL.spin(directionType::fwd, 100, velocityUnits::pct);
}

void intakeOpen(void) { //open the intakes
  IntakeR.spin(directionType::rev, 100, velocityUnits::pct);
  IntakeL.spin(directionType::rev, 100, velocityUnits::pct);
}

void stopRollers(void) {
  RollerMain.stop(brakeType::hold);
  RollerBack.stop(brakeType::hold);
}

void rollUp(void) { //roll balls up the snail
  RollerMain.spin(directionType::fwd, 100, velocityUnits::pct);
  RollerBack.spin(directionType::fwd, 100, velocityUnits::pct);
}

void rollDown(void) { //roll balls down the snail
  RollerMain.spin(directionType::rev, 100, velocityUnits::pct);
  RollerBack.spin(directionType::rev, 100, velocityUnits::pct);
}

void rollOut(void) { //roll balls out the back
  RollerMain.spin(directionType::fwd, 100, velocityUnits::pct);
  RollerBack.spin(directionType::rev, 100, velocityUnits::pct);
}

void usercontrol(void) {
  float throttle;
  float strafe;
  float turn;
  bool reject = false;
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

    bool R1 = con.ButtonR1.pressing();
    bool R2 = con.ButtonR2.pressing();
    bool L1 = con.ButtonL1.pressing();
    bool L2 = con.ButtonL2.pressing();

    reject = L1; //L1: Activate Reject Mode

    if(L2) { //L2: Roll Down
      rollDown();
    } else {stopRollers();}

    if(R1) { //R1: Intake & Roll up, or Roll Out if Reject Mode.
      intake();
      if(reject) {rollOut();}
      else {rollUp();}
    } else {stopIntakes(); stopRollers();}

    if(R2) { //R2: Open Intakes
      intakeOpen();
    } else {stopIntakes();}
  }
}
