# For imports for classes. So this is a libray file 
# import RPi.GPIO as gpio # Denne virker ikke med Rasspery pi 5
import lgpio as lgpio
from time import sleep

# Opening gpiochip 0. See gpioinfo in terminal
chip = lgpio.gpiochip_open(0)  # Open GPIO chip 0

# There may not be a need to differentiate between big and small stepper motor
class Stepper_Motor():
    def __init__(self, pulse_pin: int, dir_pin : int) -> None:
        self.pulse_pin = pulse_pin
        self.dir_pin = dir_pin
        self.steps_per_rev = 1600
        
        lgpio.gpio_claim_output(chip, pulse_pin)  # For the pulse pin
        lgpio.gpio_claim_output(chip, dir_pin)  # For the direction pin
        
        print(f"Direction pin {self.dir_pin}, pulse pin {self.pulse_pin}")

    # This function takes in the number of revolutions and the direction to turn the motor
    # clockwise is 0, counter clockwise is 1. Could be changed to a boolean.
    def opperate(self, revs: int, dir: int) -> None:
        # Setting the direction for the motor to stepin
        lgpio.gpio_write(chip, dir, 1)
        for _ in range(revs*self.steps_per_rev): # To acheive PMW
            lgpio.gpio_write(chip, self.pulse_pin, 1) # Setter pulse to HIGH 
            sleep(.00018) # Wait 18 mys or 0.00018s
            lgpio.gpio_write(chip, self.pulse_pin, 0)
            # Setter pulse to LOW
            sleep(.00018) # Wait 18 mys or 0.00018s
            
    # def clean_up(self) -> None:
    #     print("Cleaning up GPIO")
    #     lgpio.gpiochip_close(chip)
        
    def __str__(self) -> str:
        return f"Direction pin {self.dir_pin}, pulse pin {self.pulse_pin}"
    
    # def __del__(self) -> None:
    #     self.clean_up()

    def move_to_angle(self, angle: float, dir: int) -> None:
        # Calculate the number of steps needed to move to the desired angle
        steps = int((angle / 360.0) * self.steps_per_rev)
        lgpio.gpio_write(chip, dir, 1)
        for _ in range(steps):
            lgpio.gpio_write(chip, self.pulse_pin, 1)
            sleep(.00018)
            lgpio.gpio_write(chip, self.pulse_pin, 0)
            sleep(.00018)