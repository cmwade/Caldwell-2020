#include "vex.h"

using namespace vex;

vex::task positiontrackingtask;

float rWidth = 6.184; //horizontal distance from the tracking center to the right tracking wheel
float lWidth = 6.184; //horizontal distance from the tracking center to the right tracking wheel
float bLength = 6.184; //vertical distance from the tracking center to the back tracking wheel

float rWheelRatio = -0.02477; //ratio of the distance traveled to the right wheel rotation in degrees, used to calculate distance based on revolutions
float lWheelRatio = 0.02469; //ratio of the distance traveled to the left wheel rotation in degrees, used to calculate distance based on revolutions
float bWheelRatio = -0.02465; //ratio of the distance traveled to the back wheel rotation in degrees, used to calculate distance based on revolutions

float prevL = 0; //initializes the storage for the last value of the left wheel, used to calculate change
float prevR = 0; //initializes the storage for the last value of the right wheel, used to calculate change
float prevB = 0; //initializes the storage for the last value of the back wheel, used to calculate change

float prevOrientationRad = 0; //storage for the last absolute orientation in radians, used to calculate change
float prevGlobalX = 0; //initializes the storage for the last absolute x value in inches
float prevGlobalY = 0; //initializes the storage for the last absolute y value in inches
float absOrientationRad = 0; //absolute orientation in radians
float absOrientationDeg = 0; //absolute orientation in radians

float localX = 0; //x value relative to the chord of the tracking center's arc 
float localY = 0; //y value relative to the chord of the tracking center's arc

float deltaL = 0; //change in the left wheel value since last checked
float deltaR = 0; //change in the right wheel value since last checked
float deltaB = 0; //change in the back wheel value since last checked

float absGlobalX = 0; //absolute x position in inches
float absGlobalY = 0; //absolute y position in inches

float turnP = 1.2; //for later use in motion control
float driveP = 6;
float turnD = 20;
float driveD = 40;
float turnMax = 40;
float driveMax = 60;

float errorMarginBase = 1; //for later use in motion control
float errorMarginTurnDeg = 0.5;

float pi = 3.14159265359;

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

float absf( float value ) { return(sqrtf(value*value)); }

void setDriveVelocity( float LFSpeed, float LBSpeed, float RBSpeed, float RFSpeed) {
  LF.spin(directionType::fwd, LFSpeed, velocityUnits::pct);
  LB.spin(directionType::fwd, LBSpeed, velocityUnits::pct);
  RB.spin(directionType::fwd, RBSpeed, velocityUnits::pct);
  RF.spin(directionType::fwd, RFSpeed, velocityUnits::pct);
}

void driveHold( void ) {
  LF.stop(brakeType::hold);
  LB.stop(brakeType::hold);
  RB.stop(brakeType::hold);
  RF.stop(brakeType::hold);
}

void updateEncoders() {
    currentL = (EncoderL.rotation(rotationUnits::deg))*(lWheelRatio); //gets the rotation of the left wheel
    currentR = (EncoderR.rotation(rotationUnits::deg))*(rWheelRatio); //gets the rotation of the right wheel
    currentB = (EncoderB.rotation(rotationUnits::deg))*(bWheelRatio); //gets the rotation of the back wheel 
    
    deltaL = currentL - prevL; //change in the left wheel distance since last checked
    deltaR = currentR - prevR; //change in the right wheel distance since last checked
    deltaB = currentB - prevB; //change in the back wheel distance since last checked
    
    prevL = currentL; //sets up the last left wheel value for next use
    prevR = currentR; //sets up the last right wheel value for next use
    prevB = currentB; //sets up the last back wheel value for next use
}

void updatePosition() {
    absOrientationRad = (currentL-currentR)/(rWidth + lWidth); //calculates the absolute orientation of the robot in radians
    
    absOrientationDeg = reduceAngle0to360(absOrientationRad*(180/pi)); //converts to degrees for display only
    
    float deltaA = (deltaL-deltaR)/(rWidth + lWidth); // change in the orientation since last checked

    if (deltaA == 0) { //avoids dividing by zero
        localX = deltaB;
        localY = deltaR;
    } else {
        localX = (2*sin(deltaA/2))*((deltaB/deltaA)+bLength); //calculates the local change in x and y values, with the chord of the robot's arc as the positive y-axi
        localY = (2*sin(deltaA/2))*((deltaR/deltaA)+rWidth);
    }
    
    float localPolarAngle = 0; 
    float localPolarLength = 0;
    
    if (localX == 0 && localY == 0){ //prevents two zeroes from being passed to the atan2 function
        localPolarAngle = 0;
        localPolarLength = 0;
    } else {
        localPolarAngle = atan2(localY, localX); //finds the angle to convert to polar coordinates
        localPolarLength = sqrt(pow(localX, 2) + pow(localY, 2)); //finds the length to convert to polar coordinates
    }
    
    float globalPolarLength = localPolarLength; //length doesn't change when going from local to global, only rotation
    float globalPolarAngle = localPolarAngle - prevOrientationRad - (deltaA/2); //takes into account the previous rotation of the robot
    
    float globalX = globalPolarLength*cos(globalPolarAngle); //finds the global change in x since last cycle
    float globalY = globalPolarLength*sin(globalPolarAngle); //finds the global change in y since last cycle
    
    
    absGlobalX = prevGlobalX + globalX; //updates the absolute x position of the robot
    absGlobalY = prevGlobalY + globalY; //updates the absolute y position of the robot
    
    prevGlobalX = absGlobalX; //sets up the last global x value for next use
    prevGlobalY = absGlobalY; //sets up the last global y value for next use
    
    prevOrientationRad = absOrientationRad; //sets up orientation for next use
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
    Brain.Screen.drawCircle(round(Brain.timer(timeUnits::msec)/75), round(currentL)+50, 1);
  }
}

void driveReset(float X = 0, float Y = 0, float OrientationDeg = 0) {
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

void turnSlide( float endX, float endY, float endRotationDeg, float maxDriveValue = driveMax, float maxTurnValue = turnMax, float timeoutMsec = 999999, float drivePValue = driveP, float turnPValue = turnP, float driveDValue = driveD, float turnDValue = turnD, float driveErrorMargin = errorMarginBase, float turnErrorMarginDeg = errorMarginTurnDeg) {
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
    
    task::sleep(5); //changing this sleep time results in derivative values being thrown off
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
