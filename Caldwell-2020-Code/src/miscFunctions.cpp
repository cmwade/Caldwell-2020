#include "vex.h"

void openIntakes(bool waitForCompletion) { //Sinple open intakes function.
  IntakeL.resetRotation();
  IntakeR.resetRotation();
  IntakeL.startRotateTo(-240,rotationUnits::deg,100,velocityUnits::pct);
  IntakeR.rotateTo(-240,rotationUnits::deg,100,velocityUnits::pct,waitForCompletion);
}

void openIntakesWide() { //More complex open intakes function which prevents axle twisting and motor burnout.
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
  while (avgGain > 1) { //Continues to spin while the intakes are moving.
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
    avgGain = (gains[0]+gains[1]+gains[2]+gains[3]+gains[4])/5; //Calculates the average gain over the last 5 time intervals.
    task::sleep(15);
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

void unfold() { //Used to unfold the robot precisely.
  RollerBack.spin(directionType::fwd, 100, velocityUnits::pct);
  IntakeR.spin(directionType::rev,12,voltageUnits::volt);
  task::sleep(500);
  RollerBack.stop();
  IntakeL.spin(directionType::rev,12,voltageUnits::volt);
}

void eatBall() { //Closes the actuated intakes around a ball on the field.
  IntakeL.spin(directionType::fwd,100,velocityUnits::pct);
  IntakeR.spin(directionType::fwd,100,velocityUnits::pct);
  task::sleep(500);
  IntakeL.stop();
  IntakeR.stop();
}

void descoreBall( int numberOfBalls ) { //Descores the specified number of balls from a goal.
  int n = 0;
  float starttime = Brain.timer(timeUnits::msec);
  while(n < numberOfBalls) { //Repeats until a certain number of balls have been descored.
    IntakeL.stop();
    IntakeR.stop();
    RollerMain.stop();
    starttime = Brain.timer(timeUnits::msec); //Doesn't continue until the ball has both pressed and released the ball detector.
    while(BallDetector.pressing()){
      IntakeL.spin(directionType::fwd,100,velocityUnits::pct);
      IntakeR.spin(directionType::fwd,100,velocityUnits::pct);
      RollerMain.spin(directionType::fwd,100,velocityUnits::pct);
    }
    while(not(BallDetector.pressing())){
      IntakeL.spin(directionType::fwd,100,velocityUnits::pct);
      IntakeR.spin(directionType::fwd,100,velocityUnits::pct);
      RollerMain.spin(directionType::fwd,100,velocityUnits::pct);
    }
    IntakeL.stop();
    IntakeR.stop();
    RollerMain.stop();
    if (Brain.timer(timeUnits::msec) - starttime > 250) { //Prevents oscillation from counting as a false descore.
    n+=1;
    }
  }
}