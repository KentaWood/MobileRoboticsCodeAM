#ifndef POSE_ESIMATION_H
#define POSE_ESIMATION_H

// #include <ESP32Encoder.h>
#include "../../include/intervaltimer.h"
#include <Arduino.h>

const int X = 0;
const int Y = 1;
const int THETA = 2;

const unsigned long WHEEL_RADIUS = 7;   // cm
const unsigned long WHEEL_SPACING = 18; // cm

class PoseEstimation
{
public:
    unsigned long currentPosition[3]; // x, y, theta
    unsigned long estimationInterval;
    unsigned long estimationLastTime;
    signed long lastPhiR;
    signed long lastPhiL;

    PoseEstimation(unsigned long startingPosition[3], unsigned long interval) : estimationInterval(interval), estimationLastTime(0), lastPhiR(0), lastPhiL(0)
    {
        currentPosition[X] = startingPosition[X];
        currentPosition[Y] = startingPosition[Y];
        currentPosition[THETA] = startingPosition[THETA];
    }
    void setup() {
          estimationLastTime = millis();
    }

    void loopStep(bool isEnabled, unsigned long currentPhiR, unsigned long currentPhiL)
    {

        if (isEnabled)
        {
            unsigned long time = millis();
            if (time - estimationLastTime > estimationInterval)
            {
                estimationLastTime = time;

                unsigned long deltaPhiR = currentPhiR - lastPhiR;
                unsigned long deltaPhiL = currentPhiL - lastPhiL;

                unsigned long Rxdot = WHEEL_RADIUS * (deltaPhiR + deltaPhiL) / 2;
                unsigned long Rydot = 0;
                unsigned long Rthetadot = WHEEL_RADIUS * (deltaPhiR - deltaPhiL) / WHEEL_SPACING;

                lastPhiR = currentPhiR;
                lastPhiL = currentPhiL;

                unsigned long theta = currentPosition[THETA];

                currentPosition[X] += Rxdot * cos(theta) - Rydot * sin(theta);
                currentPosition[Y] += Rxdot * sin(theta) + Rydot * cos(theta);
                currentPosition[THETA] += Rthetadot;
            }
        }
    }
}

#endif // POSE_ESIMATION_H
