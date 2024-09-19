import .communication
import ..led
import ..motors
import ..display

class goToGoalClosed implements Communicator:

  state := Communicator.DISABLED
  led/Led := Led
  display/Display := Display --inverted=true
  motors/Motors := Motors

  constructor:
    led.off
    motors.stop

  on-start address port: 
    print "$address:$port"
    display.add-text "$address"
    display.add-text --y=36 "$port"
    display.add-text --y=60 "Alex + Max"
    display.draw

  on-open: enable
  on-close: disable

  on-message message:
    print "Received message: $message"
    enable

  is-enabled:
    return state == Communicator.ENABLED

  enable:
    if state == Communicator.ENABLED: return
    print "Enabling"
    state = Communicator.ENABLED
    led.on

  disable:
    if state == Communicator.DISABLED: return
    print "Disabling"
    state = Communicator.DISABLED
    led.off
    motors.stop

  goToGoal:
    forward-time := 5_000

    motor-pwm := 0.3
    last-encoder := 0

    while motor-pwm > 0:

      print "Testing $motor-pwm"
      last-encoder = motors.right-encoder.get-speed
      //last-encoder = motors.left-encoder.get-speed
      motors.set-speed-right motor-pwm
      sleep --ms=0_500
      
      if last-encoder == motors.left-encoder.get-speed:
        motor-pwm += 0.01
        print "Motor Lowest Motor PWM is $motor-pwm"
        motors.stop
        break
      
      motor-pwm -= 0.01

      // if is-enabled:
      //   print "Lets go!"
  
      //   motors.set-speed-forward 0.75
      //   sleep --ms=forward-time
      //   motors.stop
      //   break
      // else:
      //   print "Snooze"
      //   sleep --ms=3_000
  

main:
  goToGoalClosed := goToGoalClosed
  comm := WsCommunication goToGoalClosed --heartbeat-ms=100000
  print "Starting communication..."

  goToGoalClosed.goToGoal

  
      

    
