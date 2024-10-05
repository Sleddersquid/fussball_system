from library import pi5modules
import lgpio as lgpio

# Setting the GPIO pins to BCM mode.
# gpio.setmode(gpio.BCM) # This is also done in modules.py. Under revision, try to remove this line from modules.py

# Setting up the pins for the stepper motor
motor1 = pi5modules.Stepper_Motor(20, 21)
motor2 = pi5modules.Stepper_Motor(24, 23)

list_of_motors = [motor1, motor2]

try:
    while True:
        print("Clockwise")
        motor1.opperate(1, 0)
        print("Counter Clockwise")
        motor1.opperate(1, 1)

except KeyboardInterrupt:
    lgpio.gpiochip_close(pi5modules.chip)