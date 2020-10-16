#include "vex.h"

void BalltoHood ( float potValuePct, float timeoutMsec, bool intakes ) {
  while ( HoodPot.value(percentUnits::pct) < potValuePct && Brain.timer(timeUnits::msec) < timeoutMsec) {
    if (intakes) {
      IntakeR.spin(directionType::fwd, 100, velocityUnits::pct);
      IntakeL.spin(directionType::fwd, 100, velocityUnits::pct);
    }
    RollerMain.spin(directionType::fwd, 100, velocityUnits::pct);
    RollerBack.spin(directionType::fwd, 100, velocityUnits::pct);
  }
  if (intakes) {
    IntakeR.stop(coast);
    IntakeL.stop(coast);
  }
  RollerMain.stop();
  RollerBack.stop();
}

void BallReject(float timeoutMsec) {
  bool rejected = false;
  bool amRejecting = false;
  while (!rejected && Brain.timer(timeUnits::msec) < timeoutMsec) {
    IntakeL.spin(directionType::fwd, 100, velocityUnits::pct);
    IntakeR.spin(directionType::fwd, 100, velocityUnits::pct);
    if (reject) {
      amRejecting = true;
      RollerMain.spin(directionType::fwd, 100, velocityUnits::pct);
      RollerBack.spin(directionType::rev, 100, velocityUnits::pct);
    }
    else {
      if (amRejecting) {amRejecting=false; rejected=true;}
      RollerMain.spin(directionType::fwd, 100, velocityUnits::pct);
      RollerBack.spin(directionType::fwd, 100, velocityUnits::pct);
    }
  }
  RollerMain.spin(directionType::fwd, 100, velocityUnits::pct);
  RollerBack.spin(directionType::fwd, 100, velocityUnits::pct);
  task::sleep(250);
  IntakeR.stop();
  IntakeL.stop();
  RollerMain.stop();
  RollerBack.stop();
}

void scoreBall(float potValuePct, float timeoutMsec) {
  float starttime = Brain.timer(timeUnits::msec);
  while (Brain.timer(timeUnits::msec) - starttime < 1000) {
  RollerMain.spin(directionType::fwd, 100, velocityUnits::pct);
  RollerBack.spin(directionType::fwd, 100, velocityUnits::pct);
  }
  RollerMain.stop();
  RollerBack.stop();
}