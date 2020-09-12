#include "vex.h"

#define RED 0
#define BLUE 1
#define SKILLS 2

#define LEFT 0
#define RIGHT 1

#define SIMPLE 0
#define COMPLEX 1

int alliance=BLUE;
int side=LEFT;
int mode=SIMPLE;

int selectMode( void ) {
  Brain.Screen.clearScreen(color::black);
  Brain.Screen.drawRectangle(50, 50, 100, 100);
  Brain.Screen.drawRectangle(200, 50, 100, 100);
  Brain.Screen.printAt(100, 50, "COMPLEX");
  Brain.Screen.printAt(250, 50, "SIMPLE");
  int answer;
  int x;
  int y;
  bool waiting=true;
  while(waiting) {
    if (con.ButtonY.pressing()) {
      answer = SIMPLE;
      waiting = false;
    }
    if (Brain.Screen.pressing()) {
      x = Brain.Screen.xPosition();
      y = Brain.Screen.yPosition();
      if (x>50 && x<150 && y>50 && y<150) {
        answer = COMPLEX;
        waiting = false;
        while(Brain.Screen.pressing()) {
          task::sleep(50);
        }
      }
      else if (x>200 && x<300 && y>50 && y<150) {
        answer = SIMPLE;
        waiting = false;
        while(Brain.Screen.pressing()) {
          task::sleep(50);
        }
      }
    }
  }
  Brain.Screen.setCursor(1, 1);
  int returnvalue;
  switch(answer) {
    case COMPLEX:
      Brain.Screen.print("You have chosen Complex.");
      task::sleep(1000);
      Brain.Screen.clearScreen();
      returnvalue = COMPLEX;
      break;
    case SIMPLE:
      Brain.Screen.print("You have chosen Simple");
      task::sleep(1000);
      Brain.Screen.clearScreen();
      returnvalue = SIMPLE;
      break;
  }
  return returnvalue;
}

int selectAlliance( void ) {
  Brain.Screen.drawCircle(100, 100, 50, color::red);
  Brain.Screen.drawCircle(400, 100, 50, color::blue);
  Brain.Screen.drawCircle(250, 100, 50, color::purple);
  int answer;
  int x;
  int y;
  bool waiting = true;
  while(waiting) {
    if (con.ButtonY.pressing()) {
      answer = RED;
      waiting = false;
    }
    if (Brain.Screen.pressing()) {
      x = Brain.Screen.xPosition();
      y = Brain.Screen.yPosition();
      if (x>50 && x<150 && y>50 && y<150) {
        answer = RED;
        waiting = false;
        while (Brain.Screen.pressing()) {
          task::sleep(50);
        }
      }
      else if (x>350 && x<450 && y>50 && y<150) {
        answer = BLUE;
        waiting = false;
        while (Brain.Screen.pressing()) {
          task::sleep(50);
        }
      }
      else if (x>200 && x<300 && y>50 && y<150) {
        answer = SKILLS;
        waiting = false;
        while (Brain.Screen.pressing()) {
          task::sleep(50);
        }
      }
    }
  }
  int returnvalue;
  switch (answer) {
  case 1:
    Brain.Screen.clearScreen(color::blue);
    task::sleep(500);
    Brain.Screen.clearScreen(color::black);
    returnvalue = BLUE;
    break;
  case 0:
    Brain.Screen.clearScreen(color::red);
    task::sleep(500);
    Brain.Screen.clearScreen(color::black);
    returnvalue = RED;
    break;
  case 2:
    Brain.Screen.clearScreen(color::purple);
    task::sleep(500);
    Brain.Screen.clearScreen(color::black);
    returnvalue = SKILLS;
    break;
  }
  return returnvalue;
}

int selectSide( void ) {
  Brain.Screen.clearScreen(color::black);
  Brain.Screen.drawRectangle(50, 50, 100, 100);
  Brain.Screen.drawRectangle(200, 50, 100, 100);
  Brain.Screen.printAt(100, 50, "LEFT");
  Brain.Screen.printAt(250, 50, "RIGHT");
  int answer;
  int x;
  int y;
  bool waiting=true;
  while(waiting) {
    if (con.ButtonY.pressing()) {
      answer = LEFT;
      waiting = false;
    }
    if (Brain.Screen.pressing()) {
      x = Brain.Screen.xPosition();
      y = Brain.Screen.yPosition();
      if (x>50 && x<150 && y>50 && y<150) {
        answer = LEFT;
        waiting = false;
        while(Brain.Screen.pressing()) {
          task::sleep(50);
        }
      }
      else if (x>200 && x<300 && y>50 && y<150) {
        answer = RIGHT;
        waiting = false;
        while(Brain.Screen.pressing()) {
          task::sleep(50);
        }
      }
    }
  }
  Brain.Screen.setCursor(1, 1);
  bool returnvalue;
  switch(answer) {
    case LEFT:
      Brain.Screen.print("You have chosen Left.");
      task::sleep(1000);
      Brain.Screen.clearScreen();
      returnvalue = LEFT;
      break;
    case RIGHT:
      Brain.Screen.print("You have chosen Right.");
      task::sleep(1000);
      Brain.Screen.clearScreen();
      returnvalue = RIGHT;
      break;
  }
  return returnvalue;
}

void pre_auton( void ) {
  alliance = selectAlliance();
  side = selectSide();
  mode = selectMode();

  if (alliance==RED) {unwantedColor=1;}
  else if (alliance==BLUE) {unwantedColor=2;}
}
