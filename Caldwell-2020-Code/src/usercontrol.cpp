#include "vex.h"

#define AUTO 0
#define MANUAL 1
int method = AUTO;

void toggleAutoSorter(void) {
  if (method==AUTO) {method=MANUAL;}
  else {method=AUTO;}
}

void usercontrol(void) {
  float throttle;
  float strafe;
  float turn;
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

    //Intakes
    if (con.ButtonL1.pressing()) { //Intake In
      IntakeR.spin(directionType::fwd, 100, velocityUnits::pct);
      IntakeL.spin(directionType::fwd, 100, velocityUnits::pct);
    } else if (con.ButtonL2.pressing()) { //Open Intakes
      IntakeR.spin(directionType::rev, 100, velocityUnits::pct);
      IntakeL.spin(directionType::rev, 100, velocityUnits::pct);
    } else {
      IntakeR.stop(brakeType::hold);
      IntakeL.stop(brakeType::hold);
    }
    
    //Switching Roller Control Methods
    
    con.ButtonLeft.pressed(toggleAutoSorter);
    
    //Rollers
    if (con.ButtonR1.pressing() and con.ButtonR2.pressing()) { //Roll Out Back
      RollerMain.spin(directionType::fwd, 100, velocityUnits::pct);
      RollerBack.spin(directionType::rev, 100, velocityUnits::pct);
    } else if (con.ButtonR1.pressing()) { //Roll Everything In
      if (reject && method == AUTO) {
        RollerMain.spin(directionType::fwd, 100, velocityUnits::pct);
        RollerBack.spin(directionType::rev, 100, velocityUnits::pct);
      } else {
         RollerMain.spin(directionType::fwd, 100, velocityUnits::pct);
        RollerBack.spin(directionType::fwd, 100, velocityUnits::pct);
      }
    } else if (con.ButtonR2.pressing()) { //Roll Everything Out
      RollerMain.spin(directionType::rev, 100, velocityUnits::pct);
      RollerBack.spin(directionType::rev, 100, velocityUnits::pct);
    } else {
      RollerMain.stop(brakeType::coast);
      RollerBack.stop(brakeType::coast);
    }
  }
}
