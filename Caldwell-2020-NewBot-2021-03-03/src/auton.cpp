#include "vex.h"

/* AUTON SELECTOR VALUES */

#define RED 0
#define BLUE 1
#define SKILLS 2
#define LEFT 0
#define RIGHT 1
#define SIMPLE 0
#define COMPLEX 1
#define HOMEROW 2

int scorenumber = 0;
int descorenumber = 0;

void scoreDescoreSimultaneous( float score, float descore) {//This is what you put in autons.
  scorenumber = score;
  descorenumber = descore;
}

int scoredescoretask() {
  if (scorenumber !=0 || descorenumber !=0) {
    scoreDescore(scorenumber, descorenumber);
  }
  scorenumber = 0;
  descorenumber = 0;
  return 0;
}

#define INTAKES 1
#define NOINTAKES 0

int ballhoodstatus = 0;

void BalltoHoodSimltaneous( bool intakes ) {
  if (intakes) {
    ballhoodstatus = 1;
  }else{
    ballhoodstatus = 2;
  }
}

int hoodballtask() {
  if (ballhoodstatus==1) {
    BalltoHood(999999, true);
  }
  if (ballhoodstatus == 2){
    BalltoHood(999999, false);
  }
  ballhoodstatus = 0;
  return 0;
}


/*********************\
 ***               ***
 **   A U T O N S   **
 ***               ***
\*********************/

void rightHomeRow() {
  driveReset(58.4, 12.6, 297.2);
  unfold();

  //First Goal
  turnSlide(24,24,-135, 100, 100);
  IntakeL.spin(fwd, 20, pct);
  IntakeR.spin(fwd, 20, pct);
  turnSlide(14, 14, -135, 100, 100);
  IntakeL.stop(coast);
  IntakeR.stop(coast);
  goalAlign(1200, 10);
  driveReset(11.6,11.6,-135);
  spinIntakesOut();
  RollerU.spin(directionType::rev,50,velocityUnits::pct);
  RollerD.spin(directionType::rev,50,velocityUnits::pct);
  turnSlide(24,24,-135, 100, 100, 999999, driveP, turnP, driveD, turnD, 3, 3);
  RollerU.stop(coast);
  RollerD.stop(coast);

  turnSlide(72, 46, 180);
  setDriveVoltage(-8, -8, -8, -8);
  task::sleep(1000);

  //Third Goal
  turnSlide(72, 46, 180, driveMax, turnMax, 999999, driveP, turnP, driveD, turnD, 3, 3);
  turnSlide(118.5,29.5,115);
  IntakeL.spin(directionType::fwd,25,velocityUnits::pct);
  IntakeR.spin(directionType::fwd,25,velocityUnits::pct);
  goalAlign(600, 10);
  IntakeL.stop(coast);
  IntakeR.stop(coast);
  goalAlign(600,10);
  spinIntakesOut();
  task::sleep(1000);
  IntakeL.stop(coast);
  IntakeR.stop(coast);
}

void leftHomeRow() {
  driveReset(58.4, 12.6, 297.2);
  unfold();

  //First Goal
  turnSlide(24,24,-135, 100, 100);
  IntakeL.spin(fwd, 20, pct);
  IntakeR.spin(fwd, 20, pct);
  turnSlide(14, 14, -135, 100, 100);
  IntakeL.stop(coast);
  IntakeR.stop(coast);
  goalAlign(1200, 10);
  driveReset(11.6,11.6,-135);
  spinIntakesOut();
  RollerU.spin(directionType::rev,50,velocityUnits::pct);
  RollerD.spin(directionType::rev,50,velocityUnits::pct);
  turnSlide(24,24,-135, 100, 100, 999999, driveP, turnP, driveD, turnD, 3, 3);
  RollerU.stop(coast);
  RollerD.stop(coast);

  turnSlide(72, 46, 180);
  setDriveVoltage(-8, -8, -8, -8);
  task::sleep(1000);

  //Third Goal
  turnSlide(72, 46, 180, driveMax, turnMax, 999999, driveP, turnP, driveD, turnD, 3, 3);
  turnSlide(118.5,29.5,125);
  IntakeL.spin(directionType::fwd,25,velocityUnits::pct);
  IntakeR.spin(directionType::fwd,25,velocityUnits::pct);
  goalAlign(600, 10);
  IntakeL.stop(coast);
  IntakeR.stop(coast);
  goalAlign(600, 10);
  spinIntakesOut();
  task::sleep(1000);
  IntakeL.stop(coast);
  IntakeR.stop(coast);
}

void rightSimple() {
  scoreDescore(0, 1);
}

void rightComplex() {
}

void leftComplex() {
}

void homerowskills() {
}

void diagonalSkills() {
}

void autonomous(void) {
  Brain.resetTimer();

  /* START THE TASKS */
  task positiontrackingtask = task(positionTrack);
  task scoredescoretaskerere = task(scoredescoretask);
  task ballhoodtask = task(hoodballtask);

  //Determining the auton to run based on the one selected.
  if (alliance==SKILLS && mode == SIMPLE) {}
  if (alliance == SKILLS && mode == COMPLEX) {}
  else {
    if (side==LEFT) {
      if (mode==SIMPLE) {calibratePositionTrackers();}
      else if (mode==COMPLEX) {}
      else if (mode==HOMEROW) {leftHomeRow();}
    } else if (side==RIGHT) {
      if (mode==SIMPLE) {rightSimple();}
      else if (mode==COMPLEX) {}
      else if (mode==HOMEROW) {rightHomeRow();}
    }
  }
}