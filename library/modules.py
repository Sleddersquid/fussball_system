## For imports for classes
import RPi.GPIO as gpio
from time import sleep


class Rowset():
    def __init__(self, dir_pin : int, pulse_pin: int) -> None:
        self.dir_pin = dir_pin
        self.pulse_pin = pulse_pin
        self.steps_per_rev = 2000
        print(f"Direction pin {self.dir_pin}, pulse pin {self.pulse_pin}")

    def __str__(self) -> str:
        return f"Direction pin {self.dir_pin}, pulse pin {self.pulse_pin}"
        
    def opperate(self, revs: int, dir: int) -> None:
        # SEtting the direction for the motor to stepin
        gpio.output(self.dir_pin, dir)
        for _ in range(revs*self.steps_per_rev):
            gpio.output(self.pulse_pin,gpio.HIGH) # Setter pulse to HIGH 
            sleep(.001) # Venter 10 ms eller 0.001s 
            gpio.output(self.pulse_pin,gpio.LOW)
            sleep(.0005) # Venter 500mys eller 0.0005s        
            
class Stepper_Motor():
    def __init__(self) -> None:
        pass
    
    
    
    
    
