#include "kinematics.h"
#include "../../include/motorcontrol.h"
#include "../../include/wscommunicator.h"
#include "../../include/display.h"

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

//Display 
Display display();

// Motor configuration
// NOTE: Several of the fields contain arbitrary values that must be revised
// max velocity limits how much the motor will change per loop
MotorControl motorControl(0.44, 0.5, 0.5, 1.0, 0.4, 20, DELAY_INTERVAL);

// Pose Estimation configuration
const float WHEEL_RADIUS = 0.07;
const float WHEEL_SPACING = 0.19;
Kinematics kinematics(WHEEL_RADIUS, WHEEL_SPACING, DELAY_INTERVAL);

void setup(void)
{
    Serial.begin(115200);
    wsCommunicator.setup();
    motorControl.setup();
    motorControl.setTargetVelocity(0.3);
    kinematics.setup();
    display.setup();
}

void loop(void)
{
    wsCommunicator.loopStep();

    motorControl.loopStep(true);
    kinematics.loopStep(
        motorControl.getLeftVelocity(),
        motorControl.getRightVelocity());
    
    display.drawString(0, 0, "AW & ML Robot");
    display.drawString(1, 0, ("X: %f Y: %f", kinematics.x, kinematics.y) );
    display.drawString(2, 0, ("THETA: %f", kinematics.theta));

    //delay(DELAY_INTERVAL);

}
