import math

# Define a class for the robot's pose (x, y, θ)
class Pose:
    def __init__(self, x, y, θ):
        self.x = x
        self.y = y
        self.θ = θ  # In radians

# Forward kinematics function
def forward_kinematics(pose, v_left, v_right, dt, TRACK_WIDTH, wheel_radius):
    v = (v_left + v_right) / 2  
    θdot = (v_right - v_left) / TRACK_WIDTH  
    
    pose.x += v * math.cos(pose.θ) * dt
    pose.y += v * math.sin(pose.θ) * dt
    pose.θ += θdot * dt
    
    left_wheel_rotations = (v_left * dt) / (2 * math.pi * wheel_radius)
    right_wheel_rotations = (v_right * dt) / (2 * math.pi * wheel_radius)
    
    print(f"Left Wheel Rotations: {left_wheel_rotations:.2f}, Right Wheel Rotations: {right_wheel_rotations:.2f}")
    
    return pose

# Position control function
def position_control(pose, GOAL, K_POSITION, K_ORIENTATION, TRACK_WIDTH, MAX_LINEAR_VELOCITY, MAX_ANGULAR_VELOCITY):
    d = distance(pose, GOAL)  

    v = K_POSITION * d
    v = min(v, MAX_LINEAR_VELOCITY)

    angle_to_goal = angle(pose, GOAL)
    θerror = angle_to_goal - pose.θ
    θdot = K_ORIENTATION * θerror
    θdot = min(θdot, MAX_ANGULAR_VELOCITY)

    v_left = v - θdot * TRACK_WIDTH / 2
    v_right = v + θdot * TRACK_WIDTH / 2

    return v_left, v_right

# Simulation loop
def simulate_motion(pose, GOAL, TRACK_WIDTH, dt, TIME_END, K_POSITION, K_ORIENTATION, MAX_LINEAR_VELOCITY, MAX_ANGULAR_VELOCITY, wheel_radius):
    t = 0
    v_left, v_right = 0, 0

    while t < TIME_END:
        pose = forward_kinematics(pose, v_left, v_right, dt, TRACK_WIDTH, wheel_radius)
        v_left, v_right = position_control(pose, GOAL, K_POSITION, K_ORIENTATION, TRACK_WIDTH, MAX_LINEAR_VELOCITY, MAX_ANGULAR_VELOCITY)
        t += dt
        print(f"Time: {t:.2f}, Pose: x={pose.x:.2f}, y={pose.y:.2f}, θ={pose.θ:.2f}")

# Utility functions
def distance(pose, GOAL):
    return math.sqrt((GOAL[0] - pose.x)**2 + (GOAL[1] - pose.y)**2)

def angle(pose, GOAL):
    return math.atan2(GOAL[1] - pose.y, GOAL[0] - pose.x)

# Example parameters
pose = Pose(0, 0, 0)
GOAL = (10, 10)
TRACK_WIDTH = 0.07  
wheel_radius = 0.07  
dt = 0.25  
TIME_END = 20  
K_POSITION = 1.0  
K_ORIENTATION = 1.0  
MAX_LINEAR_VELOCITY = 1.0  
MAX_ANGULAR_VELOCITY = 1.0  

# Run the simulation
simulate_motion(pose, GOAL, TRACK_WIDTH, dt, TIME_END, K_POSITION, K_ORIENTATION, MAX_LINEAR_VELOCITY, MAX_ANGULAR_VELOCITY, wheel_radius)
