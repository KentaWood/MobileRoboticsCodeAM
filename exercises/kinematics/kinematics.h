#ifndef KINEMATICS_H
#define KINEMATICS_H

#include <stdio.h>
#include "../../include/intervaltimer.h"


class Kinematics
{
public:
    int x;
    int y;
    int theta;

    int wheel_radius;
    int wheel_spacing;

    IntervalTimer timer;

    Kinematics(int radius, int spacing, long interval) : x(0), y(0), theta(0), wheel_radius(radius), wheel_spacing(spacing), timer(interval) {}

    void setup()
    {
        // Nothing to set up
    }

    void loopStep(float leftVelocity, float rightVelocity, float dt)
    {
        if(timer) {
            update_position(leftVelocity, rightVelocity, dt);
        }
    }

    void update_position(float leftVelocity, float rightVelocity, float dt)
    {
        int Rxdot = wheel_radius * (rightVelocity + leftVelocity) / 2;
        int Rthetadot = wheel_radius * (rightVelocity - leftVelocity) / wheel_spacing;

        x += (Rxdot * cos(theta)) * dt;
        y += (Rxdot * sin(theta)) * dt;
        theta += Rthetadot;

        print_pose();
    }

    void print_pose()
    {
        printf("X: %d, Y: %d, THETA: %f", x, y, theta);
    }
};

#endif // KINEMATICS_H
