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
    // led.on

  disable:
    if state == Communicator.DISABLED: return
    print "Disabling"
    state = Communicator.DISABLED
    // led.off
    motors.stop

main:
  goToGoalOpen := GoToGoalOpen
  comm := WsCommunication goToGoalOpen --heartbeat-ms=1000
  print "here0"

  while true:
      if goToGoalOpen.is-enabled:
          print "here1"
          goToGoalOpen.led.on
          print "here2"

          sleep --ms=0_250
          goToGoalOpen.led.off
          sleep --ms=0_250
          print "here3"
          break
      else:
          goToGoalOpen.led.off
          break


    //   display

    
