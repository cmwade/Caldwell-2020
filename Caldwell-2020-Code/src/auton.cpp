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

/* MULTI/POLYTASKER FLAG VALUES */
#define NOTHING 0
#define UNFOLD 1
#define OPENINTAKES 2
#define OPENWIDE 3
#define BALLTOHOOD 4
#define BTHNOINTAKES 5
#define EATBALL 6
#define GOALALIGN 7
#define SCOREBALL 8
#define DESCOREBALL 9
#define DESCORETWO 10
#define DESCORETHREE 11

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
    } else if (multitasker_flag==SCOREBALL) {
      scoreBall();
      multitasker_flag=NOTHING;
    } else if (multitasker_flag==DESCOREBALL) {
      descoreBall();
      multitasker_flag=NOTHING;
    } else if (multitasker_flag==DESCORETWO) {
      descoreBall(2);
      multitasker_flag=NOTHING;
    } else if (multitasker_flag==DESCORETHREE) {
      descoreBall(3);
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
    } else if (polytasker_flag==SCOREBALL) {
      scoreBall();
      polytasker_flag=NOTHING;
    } else if (polytasker_flag==DESCOREBALL) {
      descoreBall();
      polytasker_flag=NOTHING;
    } else if (polytasker_flag==DESCORETWO) {
      descoreBall(2);
      polytasker_flag=NOTHING;
    } else if (polytasker_flag==DESCORETHREE) {
      descoreBall(3);
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
    if (supertasker_flag==UNFOLD) {
      unfold();
      supertasker_flag = NOTHING;
    } else if (supertasker_flag==OPENINTAKES) {
      openIntakes();
      supertasker_flag = NOTHING;
    } else if (supertasker_flag==OPENWIDE) {
      openIntakesWide();
      supertasker_flag = NOTHING;
    } else if (supertasker_flag==BALLTOHOOD) {
      BalltoHood();
      supertasker_flag = NOTHING;
    } else if (supertasker_flag==BTHNOINTAKES) {
      BalltoHood(20,999999,false);
      supertasker_flag=NOTHING;
    } else if (supertasker_flag==EATBALL) {
      eatBall();
      supertasker_flag=NOTHING;
    } else if (supertasker_flag==GOALALIGN) {
      goalAlign();
      supertasker_flag=NOTHING;
    } else if (supertasker_flag==SCOREBALL) {
      scoreBall();
      supertasker_flag=NOTHING;
    } else if (supertasker_flag==DESCOREBALL) {
      descoreBall();
      supertasker_flag=NOTHING;
    } else if (supertasker_flag==DESCORETWO) {
      descoreBall(2);
      supertasker_flag=NOTHING;
    } else if (supertasker_flag==DESCORETHREE) {
      descoreBall(3);
      supertasker_flag=NOTHING;
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

void rightHomeRow() {
  /*
   * Start position:
   * straddling the tile division
   * to the right of the center
   * home row goal. This goes for
   * either color.
   */
  driveReset(96, 9, 0);
  simultaneously(UNFOLD);
  turnSlide(108,36,135, 80, 40, 999999, 8, 1, 100, turnD, 3, 3);
  simultaneously(BTHNOINTAKES);
  turnSlide(122,22,135,driveMax, turnMax, 999999, driveP, 0.4, driveD, turnD, 2, 2);
  simultaneously(DESCOREBALL);
  stopIntakes();
  goalAlign(600, 8);
  simultaneously(SCOREBALL);
  goalAlign(600, 8);
  simultaneously(OPENWIDE);
  turnSlide(120,28,135, 90, 40, 999999, driveP, 1, 150, turnD, 3, 3);
  simultaneously(BALLTOHOOD);
  turnSlide(88, 28, 180, 90, 40, 999999, 10, turnP, 150, turnD, 3, 3);
  turnSlide(72, 28, 182);
  simultaneously(DESCOREBALL);
  goalAlign(600, 8);
  simultaneously(SCOREBALL);
  goalAlign(1000, 3);
  simultaneously(OPENWIDE);
  turnSlide(72, 40, 180, 80, turnMax, 999999, 10, turnP, 150, turnD, 3, 3);
  simultaneously(BALLTOHOOD);
  turnSlide(40, 40, -135, 80, turnMax, 999999, driveP, turnP, 150, turnD, 3, 3);
  turnSlide(22, 22, -135, driveMax, turnMax, 999999, driveP, turnP, driveD, turnD, 3, 3);
  simultaneously(EATBALL);
  spinIntakes();
  goalAlign(600, 8);
  simultaneously(SCOREBALL);
  goalAlign(800, 8);
  simultaneously(OPENWIDE);
  Brain.Screen.printAt(1, 160, "%c%", Brain.timer(timeUnits::msec));
  turnSlide(36, 36, -135, 100, 100, 14900, 5, turnP, 100, turnD, 5, 5);
}

void leftHomeRow() {
  /*
   * Start position:
   * straddling the tile division
   * to the right of the center
   * home row goal. This goes for
   * either color.
   */
  driveReset(48, 9, 0);
  simultaneously(UNFOLD);
  turnSlide(36,36,-135, 80, 40, 999999, 8, 1, 100, turnD, 3, 3);
  simultaneously(BTHNOINTAKES);
  turnSlide(22,22,-135,driveMax, turnMax, 999999, driveP, 0.4, driveD, turnD, 2, 2);
  simultaneously(DESCOREBALL);
  stopIntakes();
  goalAlign(600, 8);
  simultaneously(SCOREBALL);
  goalAlign(600, 8);
  simultaneously(OPENWIDE);
  turnSlide(24,28,-135, 90, 40, 999999, driveP, 1, 150, turnD, 3, 3);
  simultaneously(BALLTOHOOD);
  turnSlide(54, 28, 180, 90, 40, 999999, 10, turnP, 150, turnD, 3, 3);
  turnSlide(72, 28, 180);
  simultaneously(DESCOREBALL);
  goalAlign(600, 8);
  simultaneously(SCOREBALL);
  goalAlign(1000, 3);
  simultaneously(OPENWIDE);
  turnSlide(72, 40, 180, 80, turnMax, 999999, 10, turnP, 150, turnD, 3, 3);
  simultaneously(BALLTOHOOD);
  turnSlide(104, 40, 135, 80, turnMax, 999999, driveP, turnP, 150, turnD, 3, 3);
  turnSlide(122, 22, 135, driveMax, turnMax, 999999, driveP, turnP, driveD, turnD, 3, 3);
  simultaneously(EATBALL);
  spinIntakes();
  goalAlign(600, 8);
  simultaneously(SCOREBALL);
  goalAlign(800, 8);
  simultaneously(OPENWIDE);
  Brain.Screen.printAt(1, 160, "%c%", Brain.timer(timeUnits::msec));
  turnSlide(108, 36, 135, 100, 100, 14900, 5, turnP, 100, turnD, 5, 5);
}

void rightSimple() {
  /*
   * Start position:
   * straddling the tile division
   * to the right of the center
   * home row goal. This goes for
   * either color.
   * 
   * Hits the ball on the line 
   * and cycles the corner goal.
   */
  driveReset(96, 9, 0);
  simultaneously(UNFOLD);
  turnSlide(108,36,0, 80, 40, 999999, 8, 1, 100, turnD, 3, 3);
  simultaneously(BTHNOINTAKES);
  task::sleep(500);
  simultaneously(EATBALL);
  task::sleep(250);
  turnSlide(108, 62, 0);
  turnSlide(108,36,135, 80, 40, 999999, 8, 1, 100, turnD, 3, 3);
  turnSlide(122,22,135,driveMax, turnMax, 999999, driveP, 0.4, driveD, turnD, 2, 2);
  simultaneously(DESCORETHREE);
  goalAlign(600, 8);
  simultaneously(SCOREBALL);
  goalAlign(1000, 3);
  scoreBall();
  scoreBall();
  simultaneously(OPENWIDE);
  turnSlide(108, 36, 135, 100, 100, 14900, 5, turnP, 100, turnD, 5, 5);
}

void rightComplex() {
  /*
   * Start position:
   * straddling the tile division
   * to the right of the center
   * home row goal. This goes for
   * either color.
   *
   * Cycles the corner and middle goals.
   */
  driveReset(96, 9, 0);
  simultaneously(UNFOLD);
  turnSlide(108,36,135, 80, 40, 999999, 8, 1, 100, turnD, 3, 3);
  turnSlide(122,22,135,driveMax, turnMax, 999999, driveP, 0.4, driveD, turnD, 2, 2);
  simultaneously(DESCORETHREE);
  goalAlign(600, 8);
  simultaneously(SCOREBALL);
  goalAlign(1000, 3);
  scoreBall();
  scoreBall();
  simultaneously(OPENWIDE);
  turnSlide(120,28,135, 90, 40, 999999, driveP, 1, 150, turnD, 3, 3);
  simultaneously(SCOREBALL);
  turnSlide(88, 28, 90, 90, 40, 999999, 10, turnP, 150, turnD, 3, 3);
  turnSlide(72, 28, 180, 60, 40, 999999, driveP, turnP, driveD, turnD, 2, 2);
  simultaneously(OPENWIDE);
  goalAlign(800, 6);
  simultaneously(DESCORETWO);
  scoreBall();
  turnSlide(72, 36, 180, 100, 100, 14900, 5, turnP, 100, turnD, 5, 5);
}

void leftSimple() {
  /*
   * Start position:
   * straddling the tile division
   * to the left of the center
   * home row goal. This goes for
   * either color.
   *
   *
   */
  driveReset(48, 9, 0);
  simultaneously(UNFOLD);
  turnSlide(36,36,-135, 80, 40, 999999, 8, 1, 100, turnD, 3, 3);
  turnSlide(22,22,-135,driveMax, turnMax, 999999, driveP, 0.4, driveD, turnD, 2, 2);
  simultaneously(DESCORETHREE);
  goalAlign(600, 8);
  simultaneously(SCOREBALL);
  goalAlign(1000, 3);
  scoreBall();
  scoreBall();
  simultaneously(OPENWIDE);
  turnSlide(24,24,-135, 90, 40, 999999, driveP, 1, 150, turnD, 3, 3);
  turnSlide(24,24,0, 90, 80, 999999, driveP, 1, 150, turnD, 3, 3);
  simultaneously(EATBALL);
  RollerMain.spin(directionType::fwd, 100, velocityUnits::pct);
  RollerBack.spin(directionType::rev, 100, velocityUnits::pct);
  turnSlide(36, 61, 0);
  turnSlide(48, 61, 0);
  turnSlide(60, 61, 0, 100, 10, 14600, 6);
  turnSlide(40, 40, 0, 100, 100, 14900, 5, turnP, 100, turnD, 5, 5);
}

void leftComplex() {
  /*
   * Start position:
   * straddling the tile division
   * to the left of the center
   * home row goal. This goes for
   * either color.
   */
  driveReset(48, 9, 0);
  simultaneously(UNFOLD);
  turnSlide(36,36,-135, 80, 40, 999999, 8, 1, 100, turnD, 3, 3);
  simultaneously(BTHNOINTAKES);
  turnSlide(22,22,-135,driveMax, turnMax, 999999, driveP, 0.4, driveD, turnD, 2, 2);
  simultaneously(DESCOREBALL);
  stopIntakes();
  goalAlign(600, 8);
  simultaneously(SCOREBALL);
  goalAlign(600, 8);
  simultaneously(OPENWIDE);
  turnSlide(24,28,-135, 90, 40, 999999, driveP, 1, 150, turnD, 3, 3);
  simultaneously(BALLTOHOOD);
  turnSlide(54, 28, 180, 90, 40, 999999, 10, turnP, 150, turnD, 3, 3);
  turnSlide(72, 28, 180);
  goalAlign(600, 8);
  simultaneously(SCOREBALL);
  goalAlign(1000, 3);
  simultaneously(OPENWIDE);
  turnSlide(72, 36, 180, 100, 100, 14900, 5, turnP, 100, turnD, 5, 5);
  turnSlide(46, 36, 0, 100, 100, 14900, 5, turnP, 100, turnD, 5, 5);
  simultaneously(EATBALL);
  turnSlide(48, 61, 0);
  turnSlide(60, 61, 0, 100, 10, 14600, 6);
  turnSlide(40, 40, 0, 100, 100, 14900, 5, turnP, 100, turnD, 5, 5);
}

void autonomous(void) {
  Brain.resetTimer();
  /* START THE TASKS */
  task positiontrackingtask = task(positionTrack);
  task multitasker = task(multitasker_callback);
  task polytasker = task(polytasker_callback);
  task supertasker = task(supertasker_callback);
  if (alliance==SKILLS) {}
  else {
    if (side==LEFT) {
      if (mode==SIMPLE) {leftSimple();}
      else if (mode==COMPLEX) {leftComplex();}
      else if (mode==HOMEROW) {leftHomeRow();}
    } else if (side==RIGHT) {
      if (mode==SIMPLE) {rightSimple();}
      else if (mode==COMPLEX) {rightComplex();}
      else if (mode==HOMEROW) {rightHomeRow();}
    }
  }
}
