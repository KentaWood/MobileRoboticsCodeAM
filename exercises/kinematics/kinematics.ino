#include "kinematics.h"
#include "../../include/motorcontrol.h"
#include "../../include/wscommunicator.h"
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

// Motor configuration
// NOTE: Several of the fields contain arbitrary values that must be revised
// max velocity limits how much the motor will change per loop
MotorControl motorControl(44, 1.0, 1.0, 1.0, 5.0, 20, DELAY_INTERVAL);

// Pose Estimation configuration
const int WHEEL_RADIUS = 7;
const int WHEEL_SPACING = 19;
Kinematics kinematics(WHEEL_RADIUS, WHEEL_SPACING, DELAY_INTERVAL);

void setup(void)
{
    Serial.begin(115200);
    wsCommunicator.setup();
    motorControl.setup();
    kinematics.setup();
}

void loop(void)
{
    wsCommunicator.loopStep();

    motorControl.loopStep(wsCommunicator.isEnabled());
    kinematics.loopStep(
        motorControl.getLeftVelocity(),
        motorControl.getRightVelocity(),
        DELAY_INTERVAL);

    delay(DELAY_INTERVAL);
}
