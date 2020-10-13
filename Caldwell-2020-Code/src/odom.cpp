#include "vex.h"

//Physical Distances on the Bot
float rWidth = 5.935;
float lWidth = 5.935;

float bLength = 5.935;

//Ratios of Odom Wheels
float rWheelRatio = -0.024948;
float lWheelRatio = 0.024742; 
float bWheelRatio = -0.024603; 

//Variables to Calulate Deltas
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

//Constants for Motion Control
float turnP = 1.2;
float driveP = 6;
float turnD = 20;
float driveD = 40;
float turnMax = 40;
float driveMax = 60;
float errorMarginBase = 1;
float errorMarginTurnDeg = 0.5;

float pi = 3.14159265359;

//Encoder Values
float currentL = 0;
float currentR = 0;
float currentB = 0;

float reduceAngle0to360( float angleDeg ) {
  while(!(angleDeg >= 0 && angleDeg < 360)) {
    if( angleDeg < 0 ) { angleDeg += 360; }
    if(angleDeg >= 360) { angleDeg -= 360; }
  }
  return(angleDeg);
}

float reduceAngleMinus180to180( float angleDeg ) {
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

void goalAlign( float timeMsec, float voltage) {
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

void updateEncoders() {
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

void updatePosition() {
    absOrientationRad = (currentL-currentR)/(rWidth + lWidth); 

    absOrientationDeg = reduceAngle0to360(absOrientationRad*(180/pi)); 

    float deltaA = (deltaL-deltaR)/(rWidth + lWidth); 

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

    prevOrientationRad = absOrientationRad;
}

int positionTrack() {
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

void driveReset(float X, float Y, float OrientationDeg) {
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
  endRotationDeg = reduceAngle0to360(endRotationDeg);

  float turnError = reduceAngleMinus180to180(endRotationDeg - absOrientationDeg);
  float driveError = sqrt(pow((endX - absGlobalX), 2) + pow((endY - absGlobalY), 2));

  float deltaTurnError = 0;
  float deltaDriveError = 0;

  float prevTurnError = turnError;
  float prevDriveError = driveError;
  
  while( ((reduceAngleMinus180to180(turnError)) > turnErrorMarginDeg || (reduceAngleMinus180to180(turnError)) < -turnErrorMarginDeg || driveError > driveErrorMargin || driveError < -driveErrorMargin) && Brain.timer(timeUnits::msec) < timeoutMsec){
    turnError = reduceAngleMinus180to180(endRotationDeg - absOrientationDeg);
    driveError = sqrt(pow((endX - absGlobalX), 2) + pow((endY - absGlobalY), 2));

    deltaTurnError = turnError - prevTurnError;
    deltaDriveError = driveError - prevDriveError;

    float finalTurn = turnError*turnPValue - deltaTurnError*turnDValue;
    float finalDrive = driveError*drivePValue - deltaDriveError*driveDValue;

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
      finalDrive*(cos(absOrientationRad + atan2(endY - absGlobalY, endX - absGlobalX) - pi/4)) + finalTurn,
      finalDrive*(cos(3*pi/4-atan2(endY - absGlobalY, endX - absGlobalX)-absOrientationRad))+finalTurn,
      finalDrive*(cos(absOrientationRad + atan2(endY - absGlobalY, endX - absGlobalX) - pi/4)) - finalTurn,
      finalDrive*(cos(3*pi/4-atan2(endY - absGlobalY, endX - absGlobalX)-absOrientationRad))-finalTurn
    );

    task::sleep(5); //Changing this sleep time results in derivative values being thrown off.
  }
  driveHold();
}

void calibratePositionTrackers( void ) {
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