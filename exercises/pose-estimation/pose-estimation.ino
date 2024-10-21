#include "pose-estimation.h"
#include "../../include/motorcontrol.h"
#include "../../include/wscommunicator.h"
#include <Arduino.h>
#include <math.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

//
// Global state
//

const long DELAY_INTERVAL = 1000; // One second
const float PI = 3.141592653589793238462643383279502884L;

// Network configuration
const char *SSID = "Pomona";
const uint16_t PORT = 8181;
const unsigned long HEARTBEAT_INTERVAL = 1000;
WSCommunicator wsCommunicator(SSID, PORT, HEARTBEAT_INTERVAL);

// Motor configuration
MotorControl motorControl;

// Pose Estimation configuration
PoseEstimation poseEstimator({0, 0, 0}, DELAY_INTERVAL);

    // Removed redundant u8x8 object to prevent conflicts

void setup(void)
{
    Serial.begin(115200);
    wsCommunicator.setup();

    motorControl.setup();
    motorControl.setDirection(DIRECTION_FORWARD);
    motorControl.setPWMPercent(50);

    poseEstimator.setup();
}

void loop(void)
{
    wsCommunicator.loopStep();
    unsigned int isEnabled = wsCommunicator.isEnabled();

    motorControl.loopStep(isEnabled);
    float left_rps = motorControl.getLeftEncoder().getRotationsPerSecond();
    unsigned long left_phi = left_rps * (PI / 2);
    float right_rps = motorControl.getLeftEncoder().getRotationsPerSecond();
    unsigned long right_phi = right_rps * (PI / 2);

    poseEstimator.loopStep(isEnabled, right_phi, left_phi);
    delay(DELAY_INTERVAL); // Optional: Add a delay to control update frequency
}
