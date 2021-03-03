#include "vex.h"

void spinIntakesOut() {
  IntakeL.spin(directionType::rev,100,velocityUnits::pct);
  IntakeR.spin(directionType::rev,100,velocityUnits::pct);
}

void spinIntakesIn() {
  IntakeR.spin(directionType::fwd, 100, velocityUnits::pct);
  IntakeL.spin(directionType::fwd, 100, velocityUnits::pct);
}

void stopIntakes() {
  IntakeL.stop(hold);
  IntakeR.stop(hold);
}

void unfold(){
  RollerU.spin(directionType::fwd,100,velocityUnits::pct);
  RollerD.spin(directionType::fwd,100,velocityUnits::pct);
  IntakeR.spin(directionType::rev,100,velocityUnits::pct);
  IntakeL.spin(directionType::rev,100,velocityUnits::pct);
  task::sleep(200);
  RollerU.stop(coast);
  RollerD.stop(coast);
  IntakeL.stop(coast);
  IntakeR.stop(coast);
}

void eatBall(){
  IntakeR.spin(directionType::rev,100,velocityUnits::pct);
  IntakeL.spin(directionType::rev,100,velocityUnits::pct);
}

void spitBall(float timeoutMsec){
  RollerU.spin(directionType::rev,100,velocityUnits::pct);
  RollerD.spin(directionType::rev,100,velocityUnits::pct);
  IntakeR.spin(directionType::rev,100,velocityUnits::pct);
  IntakeL.spin(directionType::rev,100,velocityUnits::pct);
  task::sleep(timeoutMsec);
  RollerU.stop();
  RollerD.stop();
  IntakeL.stop();
  IntakeR.stop();

}