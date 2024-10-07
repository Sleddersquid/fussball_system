# For imports for classes. So this is a libray file 
import RPi.GPIO as gpio # Denne virker ikke med Rasspery pi 5
from time import sleep

# Setting up the GPIO pins to BCM mode
gpio.setmode(gpio.BCM)

# There may not be a need to differentiate between big and small stepper motor
class Stepper_Motor():
    def __init__(self, pulse_pin: int, dir_pin : int) -> None:
        self.pulse_pin = pulse_pin
        self.dir_pin = dir_pin
        self.steps_per_rev = 1600
        
        gpio.setup(pulse_pin, gpio.OUT)
        gpio.setup(dir_pin, gpio.OUT)
        
        print(f"Direction pin {self.dir_pin}, pulse pin {self.pulse_pin}")

    # This function takes in the number of revolutions and the direction to turn the motor
    # clockwise is 0, counter clockwise is 1. Could be changed to a boolean.
    def opperate(self, revs: int, dir: int) -> None:
        # Setting the direction for the motor to stepin
        gpio.output(self.dir_pin, dir)
        for _ in range(revs*self.steps_per_rev): # To acheive PMW
            gpio.output(self.pulse_pin,gpio.HIGH) # Setter pulse to HIGH 
            sleep(.000020) # Wait 18 mys or 0.00018s
            gpio.output(self.pulse_pin,gpio.LOW) # Setter pulse to LOW
            sleep(.000020) # Wait 18 mys or 0.00018s
            
    # def clean_up(self) -> None:
    #     print("Cleaning up GPIO")
    #     gpio.cleanup()
        
    def __str__(self) -> str:
        return f"Direction pin {self.dir_pin}, pulse pin {self.pulse_pin}"
    
    # def __del__(self) -> None:
    #     self.clean_up()

    # This function takes in the angle and the direction to turn the motor
    # It calculates the number of steps needed to move to an angle
    def move_to_angle(self, angle: float, dir: int) -> None:
        # Calculate the number of steps needed to move to an angle
        steps = int((angle / 360.0) * self.steps_per_rev)
        gpio.output(self.dir_pin, dir)
        for _ in range(steps):
            gpio.output(self.pulse_pin, gpio.HIGH)
            sleep(.00018)
            gpio.output(self.pulse_pin, gpio.LOW)
            sleep(.00018)
