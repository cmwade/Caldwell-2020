#include "vex.h"

void openIntakes(bool waitForCompletion) {
  IntakeL.resetRotation();
  IntakeR.resetRotation();
  IntakeL.startRotateTo(-240,rotationUnits::deg,100,velocityUnits::pct);
  IntakeR.rotateTo(-240,rotationUnits::deg,100,velocityUnits::pct,waitForCompletion);
}

void openIntakesWide() {
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
  while (avgGain > 1) {
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

void spinIntakes() {
  IntakeR.spin(directionType::fwd, 100, velocityUnits::pct);
  IntakeL.spin(directionType::fwd, 100, velocityUnits::pct);
}

void stopIntakes() {
  IntakeL.stop(coast);
  IntakeR.stop(coast);
}

void unfold() {
  RollerBack.spin(directionType::fwd, 100, velocityUnits::pct);
  IntakeR.spin(directionType::rev,12,voltageUnits::volt);
  task::sleep(500);
  RollerBack.stop();
  IntakeL.spin(directionType::rev,12,voltageUnits::volt);
}

void eatBall() {
  IntakeL.spin(directionType::fwd,100,velocityUnits::pct);
  IntakeR.spin(directionType::fwd,100,velocityUnits::pct);
}