from library import pi5modules
# For the GPIO pins for rasspberry pi 5
import lgpio

# For the ball tracker
from collections import deque
# from imutils.video import VideoStream
import numpy as np
# import cv2
# import imutils
import time

# Vekk fra bordet
def forwards(motor, steps):
    motor.steps_opperate(steps, 0) # clockwise
    
# Nærmere mot bordet, alså innover i mot bordet
def backwards(motor, steps):
    motor.steps_opperate(steps, 1) # counter clockwise

# Gjør en bevegelse fram og tilbake, n ganger, 1000 steps om gangen
def back_and_forth(motor, n):
    for i in range(0, n):
        motor.steps_opperate(1000, 1+(-1)**i)

def main():
    stor_motor = pi5modules.Big_Stepper_Motor(20, 21, "Motor1", 0) # 23, 24
    liten_motor = pi5modules.Small_Stepper_Motor(23, 24, "Motor2", 0) # 20, 21


    # DET ER IKKE SATT EN MAKS GRENSE FOR STEG PÅ MOTORNE
    # back_and_forth(stor_motor, 10)
    # back_and_forth(liten_motor, 10)
    
    # for i in range(0, 1):
        # backwards(stor_motor, 1000) # Stor motor fram først
        # forwards(liten_motor, 1000)
        # backwards(liten_motor, 1000) #liten motor 
        # forwards(stor_motor, 1000)
        # forwards(liten_motor, 1000)
        # backwards(liten_motor, 1000) #liten motor 
        # time.sleep(2)
    
    # forwards(stor_motor, 200)
    # backwards(stor_motor, 1000)
    
    # forwards(stor_motor, 200)
    
    # backwards(liten_motor, 200)
    # back_and_forth(stor_motor, 2)
    # backwards(liten_motor, 2000)


if __name__ == "__main__":
    main()
