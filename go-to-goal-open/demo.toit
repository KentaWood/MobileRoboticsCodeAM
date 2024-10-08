import .communication
import ..led
import ..motors
import ..display

class GoToGoalOpen implements Communicator:

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
    while true:
      if is-enabled:
        led.on
        sleep --ms=250
        led.off
        sleep --ms=250
      else:
        print "Snooze"
        sleep --ms=3_000
      



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
  goToGoalOpen := GoToGoalOpen
  comm := WsCommunication goToGoalOpen --heartbeat-ms=1000
  print "Starting communication..."

  goToGoalOpen.goToGoal

  
      

    
