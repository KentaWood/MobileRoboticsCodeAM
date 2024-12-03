#include "../../include/kinematics.h"
#include "../../include/positioncontrol.h"
#include "../../include/motorcontrol.h"
#include "../../include/wscommunicator.h"
#include "../../include/display.h"
#include "../../include/compass.h"

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

//Compass Object
Compass compass;
const float ALPHA = 0.5;

typedef struct {
    float x;
    float y;
} Waypoint;

float TILE_SIZE = 0.4318;
int curr_waypoint = 0;

Waypoint tile_to_meters(float tile_x, float tile_y) {
    Waypoint wp = {tile_x * TILE_SIZE, tile_y * TILE_SIZE};
    return wp;
}

Waypoint start = tile_to_meters(1, 1);
Waypoint end = tile_to_meters(20, 1);

Waypoint waypoints[] = {
        tile_to_meters(1,  9),
        tile_to_meters(10, 9),
        tile_to_meters(20, 9),
        end,
    };


void setup(void)
{
    printf("Setup Start");
    Serial.begin(115200);
    wsCommunicator.setup();
    motorControl.setup();
    kinematics.setup(start.x, start.y);
    display.setup();
    positionControl.setup();
    positionControl.set_goal(waypoints[0].x, waypoints[0].y);
    compass.setup();
    
    
    //Delay
    
    for (int count = 0; count < 1000 * 1000; count++);
    
    printf("Setup End");
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
    // printf("X: %f Y: %f\n", kinematics.x, kinematics.y);

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
    compass.loopStep();
    float compass_theta = compass.getTheta();
    float sf_theta = ALPHA * kinematics.theta + (1 - ALPHA) * compass_theta;
    kinematics.theta = sf_theta;

    // Do pose estimation
    bool shouldUpdateVelocities = positionControl.loopStep(kinematics, leftVelocity, rightVelocity);
    if(shouldUpdateVelocities){
        motorControl.setTargetVelocity(leftVelocity, rightVelocity);
    }

    if (positionControl.goal_reached) {
        // Check if the current waypoint index is within bounds
        if (curr_waypoint < sizeof(waypoints) / sizeof(Waypoint) - 1) {
            curr_waypoint++;
            positionControl.set_goal(waypoints[curr_waypoint].x, waypoints[curr_waypoint].y);
        } else {
            // Optional: handle the case when all waypoints are reached
            printf("All waypoints reached!");
        }
    }

    // // Send messages to the website
    // if (messageTimer){
    //     char message[100];
    //     snprintf(message, sizeof(message), "x=%f y=%f theta=%f vl=%f vr=%f", kinematics.x, kinematics.y,kinematics.theta,leftVelocity,rightVelocity);
    
    //     //turn the string into a list of characters send the len() string
    //     wsCommunicator.sendText(message, strlen(message));
    //     printf("X: %f Y: %f\n", kinematics.x, kinematics.y);
    //     printf("Compass THETA: %f, Final THETA: %f\n", compass_theta, kinematics.theta);
    // }
    


}
