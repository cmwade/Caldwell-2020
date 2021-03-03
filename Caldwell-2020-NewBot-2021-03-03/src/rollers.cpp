#include "vex.h"

int sensorthresholdpct = 60;
float oscillationtime = 100;
int spintime = 500;

void BalltoHood( float timeoutMsec, bool intakes ) {
  while ( HoodSensor.value(percentUnits ::pct) > sensorthresholdpct && Brain.timer(timeUnits::msec) < timeoutMsec) { //Spins everything up.
    if (intakes) {
      IntakeR.spin(directionType::fwd, 100, velocityUnits::pct);
      IntakeL.spin(directionType::fwd, 100, velocityUnits::pct);
    }
    RollerU.spin(directionType::fwd, 100, velocityUnits::pct);
    RollerD.spin(directionType::fwd, 100, velocityUnits::pct);
  }
  if (intakes) { //Stops everything after the ball exceeds the top of the robot's hood.
    IntakeR.stop(coast);
    IntakeL.stop(coast);
  }
  RollerU.stop(brakeType::hold);
  RollerD.stop(brakeType::hold);
}

void scoreDescore(float score, float descore) { 
  int snumber = 0;
  int dnumber = 0;
  bool ssensed = HoodSensor.value(pct) < sensorthresholdpct;
  bool dsensed = DescoreSensor.value(pct) < sensorthresholdpct;
  float sstarttime = Brain.timer(timeUnits::msec);
  float dstarttime = Brain.timer(timeUnits::msec);
  float extraspintime = 0;
  if (ssensed == 1) {snumber = 1;}
  while ( snumber < score || dnumber < descore) {
    if (snumber < score) {
      RollerU.spin(directionType::fwd, 100, velocityUnits::pct);
      RollerD.spin(directionType::fwd, 100, velocityUnits::pct);
    }
    if (dnumber < descore) {
      IntakeL.spin(fwd, 100, pct);
      IntakeR.spin(fwd, 100, pct);
    }
    if (snumber == score) {
      if (extraspintime == 0) {extraspintime = Brain.timer(msec);}
      if (Brain.timer(msec) - extraspintime < spintime ) {
        RollerU.spin(directionType::fwd, 100, velocityUnits::pct);
        RollerD.spin(directionType::fwd, 100, velocityUnits::pct);
      } else {
        RollerD.stop(coast);
        RollerU.stop(coast);
      }
    }
    if (snumber > score) {
      RollerD.stop(coast);
      RollerU.stop(coast);
    }
    if (dnumber >= descore) {
      IntakeL.stop(coast);
      IntakeR.stop(coast);
    }
    if (HoodSensor.value(pct) < sensorthresholdpct && ssensed == 0) {
      if (Brain.timer(msec) - sstarttime > oscillationtime) {snumber +=1;}
      sstarttime = Brain.timer(msec);
    }
    if (DescoreSensor.value(pct) < sensorthresholdpct && dsensed == 0) {
      if (Brain.timer(msec) - dstarttime > oscillationtime) {dnumber +=1;}
      dstarttime = Brain.timer(msec);
    }
    ssensed = HoodSensor.value(pct) < sensorthresholdpct;
    dsensed = DescoreSensor.value(pct) < sensorthresholdpct;
  }
  if (extraspintime == 0) {extraspintime = Brain.timer(msec);}
  while (Brain.timer(msec) - extraspintime < spintime ) {
    RollerU.spin(directionType::fwd, 100, velocityUnits::pct);
    RollerD.spin(directionType::fwd, 100, velocityUnits::pct);
  }  
  RollerU.stop(coast);
  RollerD.stop(coast);
  setDriveVoltage(0,0,0,0);//built in GoalAlign
}


bool atHood = false;
bool prevAtHood = false;
bool atReject = false;
bool prevAtReject = false;
bool nosorting=false;

