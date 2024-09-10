import .motors  // Import the motors module (local module in this case)

main:
  time-to-run := 3_000           // Time to run each motor sequence (in milliseconds)
  time-to-stop := 1_000          // Time to stop between runs (in milliseconds)
  time-between-updates := 500    // Time interval for updating motor speed display (in milliseconds)

  updates := time-to-run / time-between-updates  // Calculate how many updates should occur during one run period

  half-speed := 0.5             // Set motor speed to half of the maximum speed

  motors := Motors              // Initialize the motors (assuming this initializes or creates an instance of the Motors class)

  while true:                   // Infinite loop to keep the robot alternating between left and right turns
    print "Running turning left..."  // Indicate that the robot is turning left
    motors.left-motor.set-speed half-speed      // Set the left motor speed to 0.5 (half-speed, forward)
    motors.right-motor.set-speed -half-speed    // Set the right motor speed to -0.5 (half-speed, reverse, so it turns left)
    
    updates.repeat:   // Repeat the following block `updates` times (for the duration of `time-to-run`)
      // Print the current speed of both motors, converting to percentage by multiplying by 100
      print "Left: $(%.2f (motors.left-encoder.get-speed*100)), Right: $(%.2f (motors.right-encoder.get-speed*100))"
      sleep --ms=time-between-updates   // Pause for the specified update interval before the next iteration

    print "Stopping..."  // Indicate that the motors are stopping
    motors.stop          // Stop both motors
    sleep --ms=time-to-stop  // Wait for the specified stop duration

    print "Running turning right..."  // Indicate that the robot is turning right
    motors.left-motor.set-speed -half-speed   // Set the left motor speed to -0.5 (half-speed, reverse)
    motors.right-motor.set-speed half-speed   // Set the right motor speed to 0.5 (half-speed, forward, so it turns right)
    
    updates.repeat:   // Repeat the following block `updates` times (for the duration of `time-to-run`)
      // Print the current speed of both motors, converting to percentage by multiplying by 100
      print "Left: $(%.2f (motors.left-encoder.get-speed*100)), Right: $(%.2f (motors.right-encoder.get-speed*100))"
      sleep --ms=time-between-updates   // Pause for the specified update interval before the next iteration

    print "Stopping..."  // Indicate that the motors are stopping
    motors.stop          // Stop both motors
    sleep --ms=time-to-stop  // Wait for the specified stop duration

  motors.close   // Close or shutdown the motors safely (this line is outside the loop, but due to infinite loop, it may not be reached)
