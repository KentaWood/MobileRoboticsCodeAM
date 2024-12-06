import math
import matplotlib.pyplot as plt
import random

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

    # Lists to store x and y values
    x_vals = []
    y_vals = []

    while t < TIME_END:
        pose = forward_kinematics(pose, v_left, v_right, dt, TRACK_WIDTH, wheel_radius)
        v_left, v_right = position_control(pose, GOAL, K_POSITION, K_ORIENTATION, TRACK_WIDTH, MAX_LINEAR_VELOCITY, MAX_ANGULAR_VELOCITY)
        
        # Store values
        x_vals.append(pose.x)
        y_vals.append(pose.y)
        
        t += dt
    
    return x_vals, y_vals

# Utility functions
def distance(pose, GOAL):
    return math.sqrt((GOAL[0] - pose.x)**2 + (GOAL[1] - pose.y)**2)

def angle(pose, GOAL):
    return math.atan2(GOAL[1] - pose.y, GOAL[0] - pose.x)

# Set up 10 different combinations of parameters
parameter_combinations = [
    (random.uniform(0.5, 2.0), random.uniform(0.5, 2.0), random.uniform(0.5, 2.0), random.uniform(0.5, 2.0))
    for _ in range(10)
]

# Loop through each combination, simulate and plot the results
TRACK_WIDTH = 0.07  
wheel_radius = 0.07  
dt = 0.25  
TIME_END = 5  
GOAL = (1, 1)  # Set goal to (1, 1) as specified

for i, (K_POSITION, K_ORIENTATION, MAX_LINEAR_VELOCITY, MAX_ANGULAR_VELOCITY) in enumerate(parameter_combinations):
    # Reset the initial position for each simulation
    pose = Pose(0, 0, 0)

    # Run the simulation with the current parameter set
    x_vals, y_vals = simulate_motion(pose, GOAL, TRACK_WIDTH, dt, TIME_END, K_POSITION, K_ORIENTATION, MAX_LINEAR_VELOCITY, MAX_ANGULAR_VELOCITY, wheel_radius)

    # Plot the results for this combination
    plt.figure(figsize=(6, 6))
    plt.plot(x_vals, y_vals, c='blue', label=f'Path')
    plt.scatter(GOAL[0], GOAL[1], c='red', marker='x', label='Goal')  # Mark the goal
    plt.xlabel('x')
    plt.ylabel('y')
    plt.title(f'Simulation {i+1}\nK_POS={K_POSITION:.2f}, K_ORIENT={K_ORIENTATION:.2f}, '
              f'MAX_VEL={MAX_LINEAR_VELOCITY:.2f}, MAX_ANG_VEL={MAX_ANGULAR_VELOCITY:.2f}')
    plt.legend()
    plt.grid(True)

plt.show()