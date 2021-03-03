#include "vex.h"

double cubic(double input) { 

  //Used to ramp the turning speed, for finer control at lower speeds.

  if (input != 0) {return (pow(input,3))/(100*fabs(input));}
  else {return 0;}
}

void usercontrol(void) {
  float throttle;
  float strafe;
  float turn;

  //Ensures that the drive won't seize up when stopped.
  LF.setBrake(brakeType::coast);
  LB.setBrake(brakeType::coast);
  RB.setBrake(brakeType::coast);
  RF.setBrake(brakeType::coast);

  while(1) {
        
    //Drivebase Code

    throttle = con.Axis3.value();
    strafe = con.Axis4.value();
    turn = con.Axis1.value();

    LF.spin(directionType::fwd, (throttle+strafe+turn), velocityUnits::pct);
    LB.spin(directionType::fwd, (throttle-strafe+turn), velocityUnits::pct);
    RB.spin(directionType::fwd, (throttle+strafe-turn), velocityUnits::pct);
    RF.spin(directionType::fwd, (throttle-strafe-turn), velocityUnits::pct);

    //Intakes
    if (con.ButtonR1.pressing()) {spinIntakesIn();}
    else if (con.ButtonR2.pressing()) {spinIntakesOut();}
    else {IntakeL.stop(coast); IntakeR.stop(coast);}
    
    //Rollers (manual)
    if (con.ButtonL1.pressing()) {
      RollerU.spin(directionType::fwd, 100, velocityUnits::pct);
      RollerD.spin(directionType::fwd, 100, velocityUnits::pct);
    } 
    else if (con.ButtonL2.pressing()) {
      RollerU.spin(directionType::rev, 100, velocityUnits::pct);
      RollerD.spin(directionType::rev, 100, velocityUnits::pct);
    } 
    else {
      RollerU.stop(brakeType::hold);
      RollerD.stop(brakeType::hold);
    }

    task::sleep(15);
  }
  
}
