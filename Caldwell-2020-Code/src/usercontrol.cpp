#include "vex.h"

#define AUTO 0
#define MANUAL 1
int method = AUTO;

void toggleAutoSorter(void) {
  if (method==AUTO) {method=MANUAL;}
  else {method=AUTO;}
}

bool intakesOpening = false;

void openIntakesUSR() {
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
    /*Brain.Screen.printAt(1, 20, "%f", gains[0]);
    Brain.Screen.printAt(1, 40, "%f", gains[1]);
    Brain.Screen.printAt(1, 60, "%f", gains[2]);
    Brain.Screen.printAt(1, 80, "%f", gains[3]);
    Brain.Screen.printAt(1, 100, "%f", gains[4]);
    Brain.Screen.printAt(1,140,"%f",avgGain);*/
  }
  IntakeL.stop(hold);
  IntakeR.stop(hold);
}

void stopOpeningIntakes() {
  intakesOpening = false;
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
    con.ButtonR1.pressed(spinIntakes);
    con.ButtonR1.released(stopIntakes);
    con.ButtonR2.pressed(openIntakesWide);
    con.ButtonR2.released(stopOpeningIntakes);
    
    //Switching Roller Control Methods
    
    con.ButtonLeft.pressed(toggleAutoSorter);
    
    //Rollers
    if (con.ButtonL1.pressing() && con.ButtonL2.pressing()) { //Roll Out Back
      RollerMain.spin(directionType::fwd, 100, velocityUnits::pct);
      RollerBack.spin(directionType::rev, 100, velocityUnits::pct);
    } else if (con.ButtonL1.pressing()) { //Roll Everything In
      if (reject && method == AUTO) {
        RollerMain.spin(directionType::fwd, 100, velocityUnits::pct);
        RollerBack.spin(directionType::rev, 100, velocityUnits::pct);
      } else {
         RollerMain.spin(directionType::fwd, 100, velocityUnits::pct);
        RollerBack.spin(directionType::fwd, 100, velocityUnits::pct);
      }
    } else if (con.ButtonL2.pressing()) { //Roll Everything Out
      RollerMain.spin(directionType::rev, 100, velocityUnits::pct);
      RollerBack.spin(directionType::rev, 100, velocityUnits::pct);
    } else {
      RollerMain.stop(brakeType::coast);
      RollerBack.stop(brakeType::coast);
    }
  }
}
