#include "../../include/kinematics.h"
#include "../../include/positioncontrol.h"
#include "../../include/motorcontrol.h"
#include "../../include/wscommunicator.h"
#include "../../include/display.h"
#include <QMC5883LCompass.h>

// #include <Arduino.h>
#include <math.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

//
// Global state
//

const float DELAY_INTERVAL = 250;

// Network configuration
const char *SSID = "Pomona";
const uint16_t PORT = 8181;
const unsigned long HEARTBEAT_INTERVAL = 1000;
WsCommunicator wsCommunicator(SSID, PORT, HEARTBEAT_INTERVAL);


//message timer
const unsigned long MESSAGE_INTERVAL = 500;
IntervalTimer messageTimer(MESSAGE_INTERVAL);

//Display 
Display display;

// Motor configuration
// NOTE: Several of the fields contain arbitrary values that must be revised
// max velocity limits how much the motor will change per loop
const float MAX_LINEAR_VELOCITY = 0.4;
MotorControl motorControl(0.44, 0.5, 0.5, 1.0, MAX_LINEAR_VELOCITY, 20, DELAY_INTERVAL);

// Pose Estimation configuration
const float WHEEL_RADIUS = 0.07;
const float WHEEL_SPACING = 0.19;
Kinematics kinematics(WHEEL_RADIUS, WHEEL_SPACING, DELAY_INTERVAL);

//Position control
//poseitinoal control params
const float GOAL_X = 10;
const float GOAL_Y = 10;
const float GOAL_THRESHOLD = 0.1;

const float MAX_ANGULAR_VELOCITY = 1.0;

const float K_POSITION = 1.0;
const float K_ORIENTATION = 2.0; 

const unsigned long PROPROTIONAL_CONTROL_INTERVAL = 250;


PositionControl positionControl(GOAL_X, GOAL_Y, GOAL_THRESHOLD, MAX_LINEAR_VELOCITY, MAX_ANGULAR_VELOCITY, K_POSITION, K_ORIENTATION, WHEEL_SPACING, PROPROTIONAL_CONTROL_INTERVAL); 


QMC5883LCompass compass;
const float ALPHA = 0.5;

void setup(void)
{
    Serial.begin(115200);
    wsCommunicator.setup();
    motorControl.setup();
    kinematics.setup();
    display.setup();
    positionControl.setup();
    compass.init()
    compass.setSmoothing(10,true);

}
//
//Reset
//

void reset(){
    kinematics.x = 0;
    kinematics.y = 0;
    kinematics.theta = 0;
}

void loop(void)
{
    // Do websocket communication
    wsCommunicator.loopStep();
    if (wsCommunicator.resetFlagIsSet()){
        reset();
        wsCommunicator.clearResetFlag();
    }

    // Do motor Control
    motorControl.loopStep(wsCommunicator.isEnabled());
    float leftVelocity = motorControl.getLeftVelocity();
    float rightVelocity = motorControl.getRightVelocity();

    // Do kinemtatics
    kinematics.loopStep(leftVelocity, rightVelocity);

    // Do compass to update theta
    compass.read()
    float compass_theta = compass.getAzimuth()
    float sf_theta = ALPHA * kinematics.theta + (1 - ALPHA) * compass_theta;
    kinematics.theta = sf_theta;

    // Do pose estimation
    bool shouldUpdateVelocities = positionControl.loopStep(kinematics, leftVelocity, rightVelocity);
    if(shouldUpdateVelocities){
        motorControl.setTargetVelocity(leftVelocity, rightVelocity);
    }

    // Send messages to the website
    if (messageTimer){
        char message[100];
        snprintf(message, sizeof(message), "x=%f y=%f theta=%f vl=%f vr=%f", kinematics.x, kinematics.y,kinematics.theta,leftVelocity,rightVelocity);
    
        //turn the string into a list of characters send the len() string
        wsCommunicator.sendText(message, strlen(message));
    }
    
    printf("X: %f Y: %f\n", kinematics.x, kinematics.y);
    printf("THETA: %f\n", kinematics.theta);

}
