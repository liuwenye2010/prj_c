
# 1. Connect a green jumper wire to one side of the switch.
# 2. Connect that green jumper wire to GPIO board pin 3 (GPIO2).
# 3. Connect a black jumper wire to the other side of the switch.
# 4. Connect that black jumper wire to GPIO board pin 6 (ground).
# 5. Connect a yellow wire from board pin 37 (GPIO26).
# 6. Connect that yellow wire to the long leg of the LED.
# 7. Connect one end of the 220-ohm resistor to the short leg of the LED.
# 8. Connect a black wire to the other end of the 220-ohm resistor.
# 9. Connect the other end of the black wire to board pin 39.



#import gpiozero 
#dir gpiozero 
#help(gpiozero.LED)
#help(gpiozero.Button)
from gpiozero import LED, Button
from signal import pause

#for the pin numbering , please refer to the gpiozero api documents : https://gpiozero.readthedocs.io/en/stable/recipes.html
#Pin Numbering
#This library uses Broadcom (BCM) pin numbering for the GPIO pins, as opposed to physical (BOARD) numbering. Unlike in the RPi.GPIO library, this is not configurable. However, translation from other schemes can be used by providing prefixes to pin numbers (see below).
#Any pin marked “GPIO” in the diagram below can be used as a pin number. For example, if an LED was attached to “GPIO17” you would specify the pin number as 17 rather than 11:

# https://gpiozero.readthedocs.io/en/stable/recipes.html  for the GPIO numbering and more examples 
mySwitch = Button(2)  #GPIO2
myLED = LED(26)       #GPIO26

# test the connections
# myLED.blink()

mySwitch.when_pressed = myLED.on
mySwitch.when_released = myLED.off

pause()


