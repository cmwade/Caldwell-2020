#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;

// VEXcode device constructors

//Motors
motor LF = motor(PORT19, ratio36_1, true);
motor LB = motor(PORT9, ratio36_1, true);
motor RB = motor(PORT2, ratio36_1, false);
motor RF = motor(PORT4, ratio36_1, false);
motor IntakeR = motor(PORT6, ratio6_1, true);
motor IntakeL = motor(PORT3, ratio6_1, false);
motor RollerU = motor(PORT8, ratio6_1, false);
motor RollerD = motor(PORT7, ratio6_1, false);

//Sensors
encoder EncoderL = encoder(Brain.ThreeWirePort.A);
encoder EncoderR = encoder(Brain.ThreeWirePort.C);
encoder EncoderB = encoder(Brain.ThreeWirePort.E);
line DescoreSensor = line(Brain.ThreeWirePort.H); //Detects Balls Recently Descored
line HoodSensor = line(Brain.ThreeWirePort.G); //Detects Balls at the Hood

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
