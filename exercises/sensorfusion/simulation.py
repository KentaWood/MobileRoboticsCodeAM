import math
import random
import matplotlib.pyplot as plt

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

# Simulation loop with random additive errors
def simulate_motion(pose, GOAL, TRACK_WIDTH, dt, TIME_END, K_POSITION, K_ORIENTATION, MAX_LINEAR_VELOCITY, MAX_ANGULAR_VELOCITY, wheel_radius, random_error_range=(-0.2, 0.2)):
    t = 0
    v_left, v_right = 0, 0

    # Lists to store x and y values
    x_vals = []
    y_vals = []

    while t < TIME_END:
        v_left, v_right = position_control(pose, GOAL, K_POSITION, K_ORIENTATION, TRACK_WIDTH, MAX_LINEAR_VELOCITY, MAX_ANGULAR_VELOCITY)
        
        # Apply random additive error to each wheel's velocity
        v_left += random.uniform(*random_error_range)
        v_right += random.uniform(*random_error_range)

        pose = forward_kinematics(pose, v_left, v_right, dt, TRACK_WIDTH, wheel_radius)
        
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

# Example parameters
pose_initial = Pose(0, 0, 0)
TRACK_WIDTH = 0.07  
wheel_radius = 0.07  
dt = 0.1 
TIME_END = 5  
GOAL = (1, 1)  
K_POSITION = 0.9  
K_ORIENTATION = 1.52
MAX_LINEAR_VELOCITY = 1.21 
MAX_ANGULAR_VELOCITY = 1.13 

# Define different scenarios
scenarios = [
    ("No Error", (0, 0)),  # No random error
    ("Random Right Wheel Error", (-0.3, 0.3)),  # Random error only on the right wheel
    ("Random Left Wheel Error", (-0.2, 0.2)),  # Random error only on the left wheel
    ("Random Errors on Both Wheels", (-0.2, 0.2))  # Random error on both wheels
]

# Run simulations for each scenario
plt.figure(figsize=(10, 10))
for title, random_error in scenarios:
    pose = Pose(pose_initial.x, pose_initial.y, pose_initial.θ)  # Reset pose for each simulation
    x_vals, y_vals = simulate_motion(pose, GOAL, TRACK_WIDTH, dt, TIME_END, K_POSITION, K_ORIENTATION, MAX_LINEAR_VELOCITY, MAX_ANGULAR_VELOCITY, wheel_radius, random_error)
    
    plt.plot(x_vals, y_vals, label=f"{title}")
    plt.scatter(GOAL[0], GOAL[1], c='red', marker='x')  # Mark the goal

plt.xlabel('x')
plt.ylabel('y')
plt.title('Robot Path towards Goal with Random Additive Wheel Errors')
plt.legend()
plt.grid(True)
plt.show()
