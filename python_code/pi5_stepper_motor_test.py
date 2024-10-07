from library import pi5modules
import lgpio as lgpio
import time

# Setting the GPIO pins to BCM mode.
# gpio.setmode(gpio.BCM) # This is also done in modules.py. Under revision, try to remove this line from modules.py

# Setting up the pins for the stepper motor
motor1 = pi5modules.Big_Stepper_Motor(23, 24)
motor2 = pi5modules.Small_Stepper_Motor(20, 21)

list_of_motors = [motor1, motor2]

try:
    while True:
        print("Forward")
        # motor1.opperate(1, 0)
        motor2.move_to_angle(720, 0)
        # time.sleep(1.5)
        
        print("Backward")
        # motor1.opperate(1, 1)
        # motor2.move_to_angle(90, 1)
        # time.sleep(1.5)

except KeyboardInterrupt:
    print("Cleaning up")
    lgpio.gpiochip_close(pi5modules.chip)
