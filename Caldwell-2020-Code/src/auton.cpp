#include "vex.h"

#define RED 0
#define BLUE 1
#define SKILLS 2
#define LEFT 0
#define RIGHT 1
#define SIMPLE 0
#define COMPLEX 1

#define NOTHING 0
#define UNFOLD 1
#define OPENINTAKES 2
#define OPENWIDE 3
#define BALLTOHOOD 4
#define BTHNOINTAKES 5
#define EATBALL 6
#define GOALALIGN 7

int multitasker_flag = NOTHING;
int polytasker_flag = NOTHING;

void Test( void ) {
  driveReset(0, 0, 0);
  turnSlide(0, 24, -90);
  turnSlide(24, 24, 180);
  turnSlide(24, 0, 90);
  turnSlide(0, 0, 0);
  turnSlide(0, 24, -90);
  turnSlide(24, 24, 180);
  turnSlide(24, 0, 90);
  turnSlide(0, 0, 0);
  turnSlide(0, 24, -90);
  turnSlide(24, 24, 180);
  turnSlide(24, 0, 90);
  turnSlide(0, 0, 0);
}

int multitasker_callback() {
  while (true) {
    if (multitasker_flag==UNFOLD) {
      unfold();
      multitasker_flag = NOTHING;
    } else if (multitasker_flag==OPENINTAKES) {
      openIntakes();
      multitasker_flag = NOTHING;
    } else if (multitasker_flag==OPENWIDE) {
      openIntakesWide();
      multitasker_flag = NOTHING;
    } else if (multitasker_flag==BALLTOHOOD) {
      BalltoHood();
      multitasker_flag = NOTHING;
    } else if (multitasker_flag==BTHNOINTAKES) {
      BalltoHood(85,999999,false);
      multitasker_flag=NOTHING;
    } else if (multitasker_flag==EATBALL) {
      eatBall();
      multitasker_flag=NOTHING;
    } else if (multitasker_flag==GOALALIGN) {
      goalAlign(10);
      multitasker_flag=NOTHING;
    }
    task::sleep(25);
  }
  return 0;
}

int polytasker_callback() {
  while (true) {
    if (polytasker_flag==UNFOLD) {
      unfold();
      polytasker_flag = NOTHING;
    } else if (polytasker_flag==OPENINTAKES) {
      openIntakes();
      polytasker_flag = NOTHING;
    } else if (polytasker_flag==OPENWIDE) {
      openIntakesWide();
      polytasker_flag = NOTHING;
    } else if (polytasker_flag==BALLTOHOOD) {
      BalltoHood();
      polytasker_flag = NOTHING;
    } else if (polytasker_flag==BTHNOINTAKES) {
      BalltoHood(85,999999,false);
      polytasker_flag=NOTHING;
    } else if (polytasker_flag==EATBALL) {
      eatBall();
      polytasker_flag=NOTHING;
    } else if (polytasker_flag==GOALALIGN) {
      goalAlign(10);
      polytasker_flag=NOTHING;
    }
    task::sleep(25);
  }
  return 0;
}

void simultaneously(int flag) {
  if (multitasker_flag==NOTHING) {multitasker_flag = flag;}
  else if (polytasker_flag==NOTHING) {polytasker_flag = flag;}
}

/*********************
 ***               ***
 **   A U T O N S   **
 ***               ***
 *********************/

void fullHomeRow() {
  /*
   * Start position:
   * straddling the tile division
   * to the right of the center
   * home row goal. This goes for
   * either color.
   */
  simultaneously(OPENWIDE);
  turnSlide(24,14,135);
  simultaneously(BTHNOINTAKES);
  turnSlide(26,12,135);
  simultaneously(EATBALL);
  goalAlign(5);
  scoreBall();
  task::sleep(250);
  simultaneously(BALLTOHOOD);
  turnSlide(26,12,135);
  turnSlide(-16,6.8,216);
  scoreBall();
  simultaneously(OPENWIDE);
  turnSlide(-16,20,216,true,driveMax,turnMax,999999,driveP,turnP,driveD,turnD,errorMarginBase,errorMarginTurnDeg+5);
  turnSlide(-74.4,4,220);
  BalltoHood();
  goalAlign(5);
  scoreBall();
  simultaneously(OPENWIDE);
  turnSlide(-48,14,220);
}

void cornerMiddleR() {
  /*
   * Start position:
   * straddling the tile division
   * to the right of the center
   * home row goal. This goes for
   * either color.
   */
  simultaneously(OPENWIDE);
  turnSlide(24,14,135);
  simultaneously(BTHNOINTAKES);
  turnSlide(26,12,135);
  simultaneously(EATBALL);
  goalAlign(5);
  scoreBall();
  simultaneously(BALLTOHOOD);
  turnSlide(26,12,135);
  turnSlide(-16,6.8,216,true,driveMax,turnMax,8000);
  scoreBall();
  simultaneously(OPENINTAKES);
  turnSlide(-16,20,216,true,driveMax,turnMax,999999,driveP,turnP,driveD,turnD,errorMarginBase,errorMarginTurnDeg+5);
  eatBall();  
}

void cornerMiddleRHitball() {
  /*
   * Start position:
   * straddling the tile division
   * to the right of the center
   * home row goal. This goes for
   * either color.
   */
  simultaneously(OPENWIDE);
  turnSlide(13.5,50.2,30);
  simultaneously(BTHNOINTAKES);
  turnSlide(24,14,135);
  turnSlide(26,12,135);
  simultaneously(EATBALL);
  goalAlign(5);
  scoreBall();
  simultaneously(BALLTOHOOD);
  turnSlide(26,12,135);
  turnSlide(-16,6.8,216,true,driveMax,turnMax,12000);
  scoreBall();
  simultaneously(OPENINTAKES);
  turnSlide(-16,20,216,true,driveMax,turnMax,999999,driveP,turnP,driveD,turnD,errorMarginBase,errorMarginTurnDeg+5);
  eatBall(); 
}

void cornerMiddleL() {
  /*
   * Start position:
   * straddling the tile division
   * to the left of the center
   * home row goal. This goes for
   * either color.
   */
  simultaneously(OPENWIDE);
  turnSlide(-24,14,-135);
  simultaneously(BTHNOINTAKES);
  turnSlide(-26,12,-135);
  simultaneously(EATBALL);
  goalAlign(5);
  scoreBall();
  simultaneously(BALLTOHOOD);
  turnSlide(-26,12,-135);
  turnSlide(16,6.8,-216,true,driveMax,turnMax,8000);
  scoreBall();
  simultaneously(OPENINTAKES);
  turnSlide(16,20,-216,true,driveMax,turnMax,999999,driveP,turnP,driveD,turnD,errorMarginBase,errorMarginTurnDeg+5);
  eatBall(); 
}

void cornerMiddleCenter() {
  /*
   * Start position:
   * straddling the tile division
   * to the left of the center
   * home row goal. This goes for
   * either color.
   */
  simultaneously(OPENWIDE);
  turnSlide(-12,50.2,0,true,90,10,999999,driveP+0.5);
  simultaneously(BALLTOHOOD);
  task::sleep(250);
  turnSlide(16,6.8,-216,true,driveMax,turnMax,5000);
  simultaneously(EATBALL);
  simultaneously(GOALALIGN);
  scoreBall();
  simultaneously(OPENWIDE);
  turnSlide(16,20,216,true,driveMax,turnMax,999999,driveP,turnP,driveD,turnD,errorMarginBase+5,errorMarginTurnDeg+5);
  simultaneously(BTHNOINTAKES);
  turnSlide(-24,14,-135,true,driveMax,turnMax,999999,driveP+0.5);
  simultaneously(EATBALL);
  turnSlide(-26,12,-135);
  IntakeL.spin(fwd,20,pct);
  IntakeR.spin(fwd,20,pct);
  simultaneously(BTHNOINTAKES);
  goalAlign(10);
  scoreBall();
  simultaneously(BTHNOINTAKES);
  turnSlide(-20,17,-135,true,driveMax,turnMax,999990,driveP,turnP,driveD,turnD,errorMarginBase+5,errorMarginTurnDeg+5);
  simultaneously(OPENWIDE);
  turnSlide(12.3,53.6,38.3);
  simultaneously(GOALALIGN);
  scoreBall();
  turnSlide(-26,12,38.3,true,100,0);
}

void autonomous(void) {
  Brain.resetTimer();
  task positiontrackingtask = task(positionTrack);
  task multitasker = task(multitasker_callback);
  task polytasker = task(polytasker_callback);
  driveReset();
  if (alliance==SKILLS) {fullHomeRow();}
  else {
    if (side==LEFT) {
      if (mode==SIMPLE) {cornerMiddleL();}
      else if (mode==COMPLEX) {cornerMiddleCenter();}
    } else if (side==RIGHT) {
      if (mode==SIMPLE) {cornerMiddleR();}
      else if (mode==COMPLEX) {cornerMiddleRHitball();}
    }
  }
}
