from library import pi4modules
import RPi.GPIO as gpio
import time

# Setting the GPIO pins to BCM mode.
# gpio.setmode(gpio.BCM) # This is also done in modules.py. Under revision, try to remove this line from modules.py

# Setting up the pins for the stepper motor
motor2 = pi4modules.Stepper_Motor(20, 21)
motor1 = pi4modules.Stepper_Motor(23, 24)

list_of_motors = [motor1, motor2]
#
try:
    while True:
        print("Forward")
        motor1.opperate(1, 0)
        motor2.opperate(1, 0)
        time.sleep(0.5)
        
        print("Backward")
        motor1.opperate(1, 1)
        motor2.opperate(1, 1)
        time.sleep(0.5)

except KeyboardInterrupt:
    print("Cleaning up")
    gpio.cleanup()
