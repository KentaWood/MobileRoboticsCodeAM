#ifndef POSITION_CONTROL_H
#define POSITION_CONTROL_H


#include "../../include/intervaltimer.h"
#include "../kinematics/kinematics.h"
#include <math.h>

class PositionControl {
  
  float goalX;
  float goalY;
  float goalThreshold;

  float maxLinearVelocity;
  float maxAngularVelocity;

  float K_position;
  float K_orientation;
  IntervalTimer updateTimer;

  float trackWidth;

  
public:

bool goal_reached = false;

PositionControl(float goalX, float goalY, float goalThreshold, float maxLinearVelocity, float maxAngularVelocity, float K_position, float K_orientation,float trackWidth, unsigned long interval)
: goalX(goalX), 
  goalY(goalY), 
  goalThreshold(goalThreshold), 
  maxLinearVelocity(maxLinearVelocity), 
  maxAngularVelocity(maxAngularVelocity),
  K_position(K_position),
  K_orientation(K_orientation),
  trackWidth(trackWidth),
  updateTimer(interval) 
{}


  void setup() {}

  void set_goal(float x, float y){
    goalX = x;
    goalY = y;
  }

  

  bool loopStep(Kinematics pose, float& l_Velocity, float& r_Velocity) {
    
    goal_reached = false;
    
    if(!updateTimer){
      return false;
    }

    float d = sqrt((goalX - pose.x) * (goalX - pose.x) +  (goalY - pose.y) * (goalY - pose.y));

    //
    if (d < goalThreshold){
      r_Velocity = 0;
      l_Velocity = 0;
      goal_reached = true;
      return true;
    }

    // Proportional control for linear velocity
    float v = min(K_position * d, maxLinearVelocity);

    // Proportional control for angular velocity
    float angleToGoal = atan2(goalY - pose.y, goalX - pose.x);
    float angleError = angleToGoal - pose.theta;
    float thetaDot = min(K_orientation * angleError, maxAngularVelocity);

    // Inverse kinematics (set motor commands based on desired velocities)
    l_Velocity = v - thetaDot * trackWidth / 2.0;
    r_Velocity = v + thetaDot * trackWidth / 2.0;

    return true;

  }
};

#endif
