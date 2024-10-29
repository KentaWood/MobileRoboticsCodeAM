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
const int num_samples = 40;
float x[num_samples];
float y[num_samples];
float theta[num_samples];
int i = 0;


// Network configuration
const char *SSID = "Pomona";
const uint16_t PORT = 8181;
const unsigned long HEARTBEAT_INTERVAL = 1000;
WsCommunicator wsCommunicator(SSID, PORT, HEARTBEAT_INTERVAL);

//Display 
Display display;

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
    printf("in loop");
    if( i < num_samples ){
        wsCommunicator.loopStep();

        motorControl.loopStep(true);
        kinematics.loopStep(
            motorControl.getLeftVelocity(),
            motorControl.getRightVelocity());
        
        printf("X: %f Y: %f\n", kinematics.x, kinematics.y);
        printf("THETA: %f\n", kinematics.theta);


        x[i] = kinematics.x;
        y[i] = kinematics.y;
        theta[i] = kinematics.theta;
        i ++;
    }
    else{
        // Print all x values first
        printf("\nx values:\n");
        for (i = 0; i < num_samples; i++) {
            printf("%.2f,",  x[i]);
        }

        // Print all y values next
        printf("\ny values:\n");
        for (i = 0; i < num_samples; i++) {
            printf("%.2f,",  y[i]);
        }

        // Print all theta values last
        printf("\ntheta values:\n");
        for (i = 0; i < num_samples; i++) {
            printf("%.2f,",  theta[i]);
        }

        printf("end");

        // delay(10000000000000000);
    }

    delay(DELAY_INTERVAL);
        

}
