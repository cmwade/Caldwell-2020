#include "vex.h"

//Physical Distances on the Bot, measured in inches from the center
float rWidth = 6.01;
float lWidth = 6.01;
float bLength = 6.01;

//Ratios of Odom Wheels, in Inches/Degrees
float rWheelRatio = -0.024948;
float lWheelRatio = 0.024742; 
float bWheelRatio = -0.024603; 

//Variables to Calulate Deltas in Encoder Values
float prevL = 0;
float prevR = 0; 
float prevB = 0;

//Variables for Position
float prevOrientationRad = 0;
float prevGlobalX = 0; 
float prevGlobalY = 0; 
float absOrientationRad = 0; 
float absOrientationDeg = 0; 

//Used as a Reference for Global X and Y
float localX = 0;
float localY = 0;

//Deltas in Wheels
float deltaL = 0; 
float deltaR = 0;
float deltaB = 0; 

//Absolute Positions
float absGlobalX = 0; 
float absGlobalY = 0; 

//Constants for Motion Control, where "P" is proportional and "D" is derivative.
float turnP = 1.2;
float driveP = 6;
float turnD = 20;
float driveD = 40;
float turnMax = 40;
float driveMax = 60;
float errorMarginBase = 1;
float errorMarginTurnDeg = 0.5;

float pi = 3.14159265359; //Pi

//Encoder Values
float currentL = 0;
float currentR = 0;
float currentB = 0;

float reduceAngle0to360( float angleDeg ) { //Takes an input angle in degrees and converts it to a positive angle value below 360.
  while(!(angleDeg >= 0 && angleDeg < 360)) {
    if( angleDeg < 0 ) { angleDeg += 360; }
    if(angleDeg >= 360) { angleDeg -= 360; }
  }
  return(angleDeg);
}

float reduceAngleMinus180to180( float angleDeg ) { //Takes an input angle in degrees and converts it to an angle between -180 and 180.
  while(!(angleDeg >= -180 && angleDeg < 180)) {
    if( angleDeg < -180 ) { angleDeg += 360; }
    if(angleDeg >= 180) { angleDeg -= 360; }
  }
  return(angleDeg);
}

void setDriveVelocity( float LFSpeed, float LBSpeed, float RBSpeed, float RFSpeed) {
  LF.spin(directionType::fwd, LFSpeed, velocityUnits::pct);
  LB.spin(directionType::fwd, LBSpeed, velocityUnits::pct);
  RB.spin(directionType::fwd, RBSpeed, velocityUnits::pct);
  RF.spin(directionType::fwd, RFSpeed, velocityUnits::pct);
}

void setDriveVoltage( float LFSpeed, float LBSpeed, float RBSpeed, float RFSpeed) {
  LF.spin(directionType::fwd, LFSpeed, voltageUnits::volt);
  LB.spin(directionType::fwd, LBSpeed, voltageUnits::volt);
  RB.spin(directionType::fwd, RBSpeed, voltageUnits::volt);
  RF.spin(directionType::fwd, RFSpeed, voltageUnits::volt);
}

void goalAlign( float timeMsec, float voltage) { //Drives the robot into a goal for a specified time at a specified voltage.
  float startingtime = Brain.timer(timeUnits::msec);
  while ( Brain.timer(timeUnits::msec) - startingtime < timeMsec) {
    setDriveVoltage(voltage, voltage, voltage, voltage);
  }
  setDriveVoltage(0,0,0,0);
}

void driveHold( void ) {
  LF.stop(brakeType::hold);
  LB.stop(brakeType::hold);
  RB.stop(brakeType::hold);
  RF.stop(brakeType::hold);
}

void updateEncoders() { //Calculates the change in encoder values from the last cycle.
    currentL = (EncoderL.rotation(rotationUnits::deg))*(lWheelRatio); 
    currentR = (EncoderR.rotation(rotationUnits::deg))*(rWheelRatio); 
    currentB = (EncoderB.rotation(rotationUnits::deg))*(bWheelRatio); 

    deltaL = currentL - prevL; 
    deltaR = currentR - prevR;
    deltaB = currentB - prevB;

    prevL = currentL;
    prevR = currentR;
    prevB = currentB; 
}

void updatePosition() { //Approximates the motion of the robot as an arc, and updates its position accordingly
    absOrientationRad = (currentL-currentR)/(rWidth + lWidth); 

    absOrientationDeg = reduceAngle0to360(absOrientationRad*(180/pi)); 

    float deltaA = (deltaL-deltaR)/(rWidth + lWidth); //Change in angle.

    //Calulates coordinates relative to the chord of the robot's arc.
    if (deltaA == 0) { //Prevents divide by zero error.
        localX = deltaB;
        localY = deltaR;
    } else {
        localX = (2*sin(deltaA/2))*((deltaB/deltaA)+bLength); 
        localY = (2*sin(deltaA/2))*((deltaR/deltaA)+rWidth);
    }

    float localPolarAngle = 0;
    float localPolarLength = 0;

    //Caluclates polar coordinates.
    if (localX == 0 && localY == 0){ //Prevents two zeros from being passed to atan2.
        localPolarAngle = 0;
        localPolarLength = 0;
    } else {
        localPolarAngle = atan2(localY, localX); 
        localPolarLength = sqrt(pow(localX, 2) + pow(localY, 2)); 
    }

    //Converts polar coordinates to global coordinates.
    float globalPolarLength = localPolarLength; 
    float globalPolarAngle = localPolarAngle - prevOrientationRad - (deltaA/2);

    float globalX = globalPolarLength*cos(globalPolarAngle); 
    float globalY = globalPolarLength*sin(globalPolarAngle); 

    //Calulates absolute position and orientation.
    absGlobalX = prevGlobalX + globalX; 
    absGlobalY = prevGlobalY + globalY; 

    prevGlobalX = absGlobalX;
    prevGlobalY = absGlobalY;

    //Sets up the previous orientation for deltas.
    prevOrientationRad = absOrientationRad;
}

int positionTrack() { //Background thread used to position track full time.
  while(1){
    updateEncoders();
    updatePosition();

    Brain.Screen.printAt(1, 20, "Absolute X: %f Inches", absGlobalX);
    Brain.Screen.printAt(1, 40, "Absolute Y: %f Inches", absGlobalY);
    Brain.Screen.printAt(1, 60, "Absolute Rotation: %f Radians, %f Degrees", absOrientationRad, absOrientationDeg);
    Brain.Screen.printAt(1, 80, "Left Wheel: %f Inches, %f Degrees", currentL, EncoderL.rotation(vex::rotationUnits::deg));
    Brain.Screen.printAt(1, 100, "Right Wheel: %f Inches, %f Degrees", currentR, EncoderR.rotation(vex::rotationUnits::deg));
    Brain.Screen.printAt(1, 120, "Back Wheel: %f Inches, %f Degrees", currentB, EncoderB.rotation(vex::rotationUnits::deg));
  }
}

void driveReset(float X, float Y, float OrientationDeg) { //Tells the robot its position on the field at the beginning of a match.
  EncoderR.setRotation(0, rotationUnits::deg);
  EncoderL.setRotation((rWidth+lWidth)*(pi/180)*(OrientationDeg)/(lWheelRatio), rotationUnits::deg);
  EncoderB.setRotation(0, rotationUnits::deg);

  prevL = 0;
  prevR = 0;
  prevB = 0;

  updateEncoders();
  updatePosition();

  prevGlobalX = 0;
  prevGlobalY = 0;

  prevGlobalX = X;
  prevGlobalY = Y;
}

void turnSlide(float endX, float endY, float endRotationDeg, float maxDriveValue, float maxTurnValue, float timeoutMsec, float drivePValue, float turnPValue, float driveDValue, float turnDValue, float driveErrorMargin, float turnErrorMarginDeg) {

  //Using a PD controller, simultaneously turns and drives to a point.

  endRotationDeg = reduceAngle0to360(endRotationDeg);

  float turnError = reduceAngleMinus180to180(endRotationDeg - absOrientationDeg);
  float driveError = sqrt(pow((endX - absGlobalX), 2) + pow((endY - absGlobalY), 2)); //Drive error is the absolute distance from the tracking center to the desired point, calculated using the Pythagorean Theorem.

  float deltaTurnError = 0;
  float deltaDriveError = 0;

  float prevTurnError = turnError;
  float prevDriveError = driveError;
  
  while( ((reduceAngleMinus180to180(turnError)) > turnErrorMarginDeg || (reduceAngleMinus180to180(turnError)) < -turnErrorMarginDeg || driveError > driveErrorMargin || driveError < -driveErrorMargin) && Brain.timer(timeUnits::msec) < timeoutMsec){
    turnError = reduceAngleMinus180to180(endRotationDeg - absOrientationDeg);
    driveError = sqrt(pow((endX - absGlobalX), 2) + pow((endY - absGlobalY), 2)); //Absolute distance from the tracking center to the desired point.

    deltaTurnError = turnError - prevTurnError;
    deltaDriveError = driveError - prevDriveError;

    float finalTurn = turnError*turnPValue - deltaTurnError*turnDValue; //Final value to pass to the motor, taking P and D loops into account.
    float finalDrive = driveError*drivePValue - deltaDriveError*driveDValue; //Final drive value to pass to the motor, taking P and D loops into account.

    prevTurnError = turnError;
    prevDriveError = driveError;

    //Prevents large values from being passed.
    if( finalTurn > maxTurnValue ) {
      finalTurn = maxTurnValue;
    }
    if( finalTurn < -maxTurnValue) {
      finalTurn = -maxTurnValue;
    }
    if( finalDrive > maxDriveValue ) {
      finalDrive = maxDriveValue;
    }
    if( finalDrive < -maxDriveValue ) {
      finalDrive = -maxDriveValue;
    }
    setDriveVelocity(
      finalDrive*(cos(absOrientationRad + atan2(endY - absGlobalY, endX - absGlobalX) - pi/4)) + finalTurn, // The robot uses trigonometry to determine the ideal value to pass to a motor
      finalDrive*(cos(3*pi/4-atan2(endY - absGlobalY, endX - absGlobalX)-absOrientationRad))+finalTurn,     // given each wheel's 45 degree offset. This value for the drive is then added
      finalDrive*(cos(absOrientationRad + atan2(endY - absGlobalY, endX - absGlobalX) - pi/4)) - finalTurn, // to the value for turning, which is negative on the right side of the bot.
      finalDrive*(cos(3*pi/4-atan2(endY - absGlobalY, endX - absGlobalX)-absOrientationRad))-finalTurn
    );

    task::sleep(5); //Changing this sleep time results in derivative values being thrown off.
  }
  driveHold();
}

void calibratePositionTrackers( void ) { //Used to find the most exact values for distances between encoders and wheel ratios.
  Brain.Screen.clearScreen();
  while(not(Brain.Screen.pressing())){
    Brain.Screen.printAt(1, 20, "Move the robot forward 96 inches. Tap to begin. ");
  }
  while(Brain.Screen.pressing()){ }

  driveReset(0, 0, 0);
  Brain.Screen.clearScreen();
  while(not(Brain.Screen.pressing())){
    Brain.Screen.printAt(1, 20, "Move the robot forward 96 inches. Tap to end.");
  }
  while(Brain.Screen.pressing()){ }
  float leftwheelratio = 96/EncoderL.rotation(vex::rotationUnits::deg);
  float rightwheelratio = 96/EncoderR.rotation(vex::rotationUnits::deg);

  Brain.Screen.clearScreen();
  while(not(Brain.Screen.pressing())){
    Brain.Screen.printAt(1, 20, "Move the robot right 96 inches. Tap to begin. ");
  }
  while(Brain.Screen.pressing()){ }

  driveReset(0, 0, 0);
  Brain.Screen.clearScreen();
  while(not(Brain.Screen.pressing())){
    Brain.Screen.printAt(1, 20, "Move the robot right 96 inches. Tap to end.");
  }
  while(Brain.Screen.pressing()){ }
  float backwheelratio = 96/EncoderB.rotation(vex::rotationUnits::deg);

  Brain.Screen.clearScreen();
  while(not(Brain.Screen.pressing())){
    Brain.Screen.printAt(1, 20, "Spin the robot clockwise four times. Tap to begin. ");
  }
  while(Brain.Screen.pressing()){ }

  driveReset(0, 0, 0);
  Brain.Screen.clearScreen();
  while(not(Brain.Screen.pressing())){
    Brain.Screen.printAt(1, 20, "Spin the robot clockwise four times. Tap to end.");
  }
  while(Brain.Screen.pressing()){ }
  float drivedistance = ((EncoderL.rotation(rotationUnits::deg))*(leftwheelratio)-(EncoderR.rotation(rotationUnits::deg))*(rightwheelratio))/(16*pi);

  Brain.Screen.clearScreen();
  while(not(Brain.Screen.pressing())){
    Brain.Screen.printAt(1, 20, "lWheelRatio: %f", leftwheelratio);
    Brain.Screen.printAt(1, 40, "rWheelRatio: %f", rightwheelratio);
    Brain.Screen.printAt(1, 60, "bWheelRatio: %f", backwheelratio);
    Brain.Screen.printAt(1, 80, "Drive Distance (all): %f", drivedistance);
  }
  while(Brain.Screen.pressing()){ }
}
