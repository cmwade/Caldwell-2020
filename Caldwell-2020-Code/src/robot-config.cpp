#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;

// VEXcode device constructors

//Motors
motor LF = motor(PORT12, ratio18_1, false);
motor LB = motor(PORT11, ratio18_1, false);
motor RB = motor(PORT20, ratio18_1, true);
motor RF = motor(PORT19, ratio18_1, true);
motor IntakeR = motor(PORT18, ratio36_1, true);
motor IntakeL = motor(PORT13, ratio36_1, false);
motor RollerBack = motor(PORT9, ratio18_1, false);
motor RollerMain = motor(PORT10, ratio18_1, true);

//Sensors
encoder EncoderL = encoder(Brain.ThreeWirePort.C);
encoder EncoderR = encoder(Brain.ThreeWirePort.E);
encoder EncoderB = encoder(Brain.ThreeWirePort.G);
limit BallDetector = limit(Brain.ThreeWirePort.B); //Detects Balls Recently Descored
pot HoodPot = pot(Brain.ThreeWirePort.A); //Detects Balls at the Hood

controller con = controller(primary);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  // Nothing to initialize
}
