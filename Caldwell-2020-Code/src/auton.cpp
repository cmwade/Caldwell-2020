#include "vex.h"

/* AUTON SELECTOR VALUES */
#define RED 0
#define BLUE 1
#define SKILLS 2
#define LEFT 0
#define RIGHT 1
#define SIMPLE 0
#define COMPLEX 1

/* MULTI/POLYTASKER FLAG VALUES */
#define NOTHING 0
#define UNFOLD 1
#define OPENINTAKES 2
#define OPENWIDE 3
#define BALLTOHOOD 4
#define BTHNOINTAKES 5
#define EATBALL 6
#define GOALALIGN 7

/* FLAGS FOR MULTI/POLYTASKER */
int multitasker_flag = NOTHING;
int polytasker_flag = NOTHING;
int supertasker_flag = NOTHING;

void Test( void ) { // A test auton.
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
  /*
   * This is the callback for the Multitasker thread.
   * It waits until the variable multitasker_flag is changed
   * to a value, and then calls the function associated with that
   * value and sets multitasker_flag back to NOTHING.
   */
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
      BalltoHood(20,999999,false);
      multitasker_flag=NOTHING;
    } else if (multitasker_flag==EATBALL) {
      eatBall();
      multitasker_flag=NOTHING;
    } else if (multitasker_flag==GOALALIGN) {
      goalAlign();
      multitasker_flag=NOTHING;
    }
    task::sleep(25);
  }
  return 0;
}

int polytasker_callback() {
  /*
   * This is the callback for the Polytasker thread.
   * It pretty much does the same thing as the Multitasker
   * thread, except it looks at the variable polytasker_flag
   * instead. This is just so that we can have 2 functions
   * running in the background instead of just 1.
   */
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
      BalltoHood(20,999999,false);
      polytasker_flag=NOTHING;
    } else if (polytasker_flag==EATBALL) {
      eatBall();
      polytasker_flag=NOTHING;
    } else if (polytasker_flag==GOALALIGN) {
      goalAlign();
      polytasker_flag=NOTHING;
    }
    task::sleep(25);
  }
  return 0;
}

int supertasker_callback() {
  /*
   * Another Callback Thread
   */
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
      BalltoHood(20,999999,false);
      multitasker_flag=NOTHING;
    } else if (multitasker_flag==EATBALL) {
      eatBall();
      multitasker_flag=NOTHING;
    } else if (multitasker_flag==GOALALIGN) {
      goalAlign();
      multitasker_flag=NOTHING;
    }
    task::sleep(25);
  }
  return 0;
}

void simultaneously(int flag) {
  /*
   * Assigns the flag to the Multitasker thread, or if the thread is busy,
   * to the Polytasker thread, or if the thread is busy, the Supertasker thread.
   */
  if (multitasker_flag==NOTHING) {multitasker_flag = flag;}
  else if (polytasker_flag==NOTHING) {polytasker_flag = flag;}
  else if (supertasker_flag==NOTHING) {supertasker_flag = flag;}
}

/*********************\
 ***               ***
 **   A U T O N S   **
 ***               ***
\*********************/

void fullHomeRow() {
  /*
   * Start position:
   * straddling the tile division
   * to the right of the center
   * home row goal. This goes for
   * either color.
   */
  driveReset(96, 9, 0);
  simultaneously(UNFOLD);
  turnSlide(108,36,135, 70, 40, 999999, 5, 1, 55, turnD, 3, 3);
  simultaneously(BTHNOINTAKES);
  simultaneously(EATBALL);
  turnSlide(128,16,135,driveMax, turnMax, 999999, driveP, 0.4, driveD, turnD, 2, 2);
  stopIntakes();
  goalAlign();
  scoreBall();
  simultaneously(BALLTOHOOD);
  turnSlide(120,24,135, 70, 40, 999999, 5, 1, 55, turnD, 3, 3);
  turnSlide(72, 24, 180);
  simultaneously(OPENWIDE);
  goalAlign(500);
  scoreBall();
  turnSlide(72, 40, 180, 80, turnMax, 999999, driveP, turnP, driveD, turnD, 3, 3);
  turnSlide(40, 40, -135);
  simultaneously(EATBALL);
  turnSlide(16, 16, -135);
  BalltoHood();
  goalAlign();
  scoreBall();
  simultaneously(OPENWIDE);
  turnSlide(24, 24, -135);
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
  turnSlide(-16,6.8,216,driveMax,turnMax,8000);
  scoreBall();
  simultaneously(OPENINTAKES);
  turnSlide(-16,20,216,driveMax,turnMax,999999,driveP,turnP,driveD,turnD,errorMarginBase,errorMarginTurnDeg+5);
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
  turnSlide(-16,6.8,216,driveMax,turnMax,12000);
  scoreBall();
  simultaneously(OPENINTAKES);
  turnSlide(-16,20,216,driveMax,turnMax,999999,driveP,turnP,driveD,turnD,errorMarginBase,errorMarginTurnDeg+5);
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
  turnSlide(16,6.8,-216,driveMax,turnMax,8000);
  scoreBall();
  simultaneously(OPENINTAKES);
  turnSlide(16,20,-216,driveMax,turnMax,999999,driveP,turnP,driveD,turnD,errorMarginBase,errorMarginTurnDeg+5);
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
  turnSlide(-12,50.2,0,90,10,999999,driveP+0.5);
  simultaneously(BALLTOHOOD);
  task::sleep(250);
  turnSlide(16,6.8,-216,driveMax,turnMax,5000);
  simultaneously(EATBALL);
  simultaneously(GOALALIGN);
  scoreBall();
  simultaneously(OPENWIDE);
  turnSlide(16,20,216,driveMax,turnMax,999999,driveP,turnP,driveD,turnD,errorMarginBase+5,errorMarginTurnDeg+5);
  simultaneously(BTHNOINTAKES);
  turnSlide(-24,14,-135,driveMax,turnMax,999999,driveP+0.5);
  simultaneously(EATBALL);
  turnSlide(-26,12,-135);
  IntakeL.spin(fwd,20,pct);
  IntakeR.spin(fwd,20,pct);
  simultaneously(BTHNOINTAKES);
  goalAlign(10);
  scoreBall();
  simultaneously(BTHNOINTAKES);
  turnSlide(-20,17,-135,driveMax,turnMax,999990,driveP,turnP,driveD,turnD,errorMarginBase+5,errorMarginTurnDeg+5);
  simultaneously(OPENWIDE);
  turnSlide(12.3,53.6,38.3);
  simultaneously(GOALALIGN);
  scoreBall();
  turnSlide(-26,12,38.3,100,0);
}

void autonomous(void) {
  Brain.resetTimer();
  /* START THE TASKS */
  task positiontrackingtask = task(positionTrack);
  task multitasker = task(multitasker_callback);
  task polytasker = task(polytasker_callback);
  task supertasker = task(supertasker_callback);
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
