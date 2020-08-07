#include "vex.h"

void BalltoHood ( float potValuePct, float timeoutMsec ) {
  while ( HoodPot.value(percentUnits::pct) < potValuePct and Brain.timer(timeUnits::msec) < timeoutMsec) {
    IntakeR.spin(directionType::fwd, 100, velocityUnits::pct);
    IntakeL.spin(directionType::fwd, 100, velocityUnits::pct);
    RollerMain.spin(directionType::fwd, 100, velocityUnits::pct);
    RollerBack.spin(directionType::rev, 100, velocityUnits::pct);
  }
}