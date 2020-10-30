#include "vex.h"

#define NO_BALL 0
#define BLUE 1
#define RED 2

#define AUTO 0
#define MANUAL 1
int method = AUTO;

bool bth = false;

void toggleAutoSorter(void) {
  if (method==AUTO) {method=MANUAL;}
  else {method=AUTO;}
}

double cubic(double input) { 

  //Used to ramp the turning speed, for finer control at lower speeds.

  if (input != 0) {return (pow(input,3))/(100*fabs(input));}
  else {return 0;}
}

bool intakesOpening = false;

void openIntakesUSR() {

  //Used to open the intakes without overheating motors or twisting axles.

  IntakeL.spin(directionType::rev,12,voltageUnits::volt);
  IntakeR.spin(directionType::rev,12,voltageUnits::volt);
  float gains[5] = {100,100,100,100,100};
  float avgGain = (gains[0]+gains[1]+gains[2]+gains[3]+gains[4])/5;
  float lGain;
  float rGain;
  float totalGain;
  float lStart;
  float rStart;
  Brain.Screen.clearScreen();
  intakesOpening = true;
  while (avgGain > 5 && intakesOpening) {
    lStart = IntakeL.rotation(rotationUnits::deg);
    rStart = IntakeR.rotation(rotationUnits::deg);
    task::sleep(25);
    lGain = fabs(IntakeL.rotation(deg)-lStart);
    rGain = fabs(IntakeR.rotation(deg)-rStart);
    totalGain = (lGain+rGain)/2;
    for (int n=0;n<4;n++) {
      gains[n] = gains[n+1];
    }
    gains[4] = totalGain;
    avgGain = (gains[0]+gains[1]+gains[2]+gains[3]+gains[4])/5;
    task::sleep(15);
  }
  IntakeL.stop(hold);
  IntakeR.stop(hold);
}

void stopOpeningIntakes() {
  intakesOpening = false;
  IntakeL.stop(hold);
  IntakeR.stop(hold);
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
    turn = cubic(con.Axis1.value());

    LF.spin(directionType::fwd, (throttle+strafe+turn), velocityUnits::pct);
    LB.spin(directionType::fwd, (throttle-strafe+turn), velocityUnits::pct);
    RB.spin(directionType::fwd, (throttle+strafe-turn), velocityUnits::pct);
    RF.spin(directionType::fwd, (throttle-strafe-turn), velocityUnits::pct);

    //Intake and Roller Code

    //Intakes
    con.ButtonL1.pressed(spinIntakes);
    con.ButtonL1.released(stopIntakes);
    con.ButtonL2.pressed(openIntakesUSR);
    con.ButtonL2.released(stopOpeningIntakes);
    con.ButtonA.pressed(unfold);
    
    //Switching Roller Control Methods
    
    con.ButtonLeft.pressed(toggleAutoSorter);
    
    //Ball to Hood
    if (con.ButtonB.pressing()) {
      bth = true;
    }
    
    //Rollers
    if (con.ButtonR1.pressing() && con.ButtonR2.pressing()) { //Ejects Balls
      RollerMain.spin(directionType::fwd, 100, velocityUnits::pct);
      RollerBack.spin(directionType::rev, 100, velocityUnits::pct);
    } else if (con.ButtonR1.pressing()) { //Scores Balls
      RollerMain.spin(directionType::fwd, 100, velocityUnits::pct);
      RollerBack.spin(directionType::fwd, 100, velocityUnits::pct);
    } else if (con.ButtonR2.pressing()) { //Pushes Balls Down
      RollerMain.spin(directionType::rev, 100, velocityUnits::pct);
      RollerBack.spin(directionType::rev, 100, velocityUnits::pct);
    } else if ( HoodPot.value(percentUnits::pct) < 20 && bth == true) { //Autonmatically Brings Balls to the Hood
      RollerMain.spin(directionType::fwd, 100, velocityUnits::pct);
      RollerBack.spin(directionType::fwd, 100, velocityUnits::pct);
    } else { //Stops the Rollers
      bth = false;
      RollerMain.stop(brakeType::hold);
      RollerBack.stop(brakeType::hold);
    }
  }
}
