#ifndef KINEMATICS_H
#define KINEMATICS_H

#include <stdio.h>
#include "../../include/intervaltimer.h"


class Kinematics
{
public:
    float x;
    float y;
    float theta;

    float wheel_radius;
    float wheel_spacing;

    IntervalTimer timer;

    Kinematics(float radius, float spacing, unsigned long interval) : x(0.0), y(0.0), theta(0.0), wheel_radius(radius), wheel_spacing(spacing), timer(interval) {}

    void setup()
    {
        // Nothing to set up
    }

    void loopStep(float leftVelocity, float rightVelocity)
    {
        if(timer) {
            update_position(leftVelocity, rightVelocity, ((float)timer.getLastDelta())/1000.0);
        }
    }

    void update_position(float leftVelocity, float rightVelocity, float dt)
    {
        float Rxdot =  (rightVelocity + leftVelocity) / 2;
        float Rthetadot = (rightVelocity - leftVelocity) / wheel_spacing;

        // printf("Rxdot: %f, Rthetadot: %f\n", Rxdot, Rthetadot);

        x += (Rxdot * cos(theta)) * dt;
        y += (Rxdot * sin(theta)) * dt;
        theta += Rthetadot * dt;

        // print_pose();
        // printf("X: %f Y: %f, THETA: %f\n", x, y, theta);
        // printf("L VEL: %f, R VEL: %f, DT: %f\n", leftVelocity, rightVelocity, dt);
    }

    // void print_pose()
    // {
    //     printf("X: %f Y: %f, THETA: %f\n", x, y, theta);
    // }
};

#endif // KINEMATICS_H
