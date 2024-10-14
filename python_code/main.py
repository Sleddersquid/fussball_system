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


def forwards(motor, steps):
    motor.steps_opperate(steps, 0)
    
    
def backwards(motor, steps):
    motor.steps_opperate(steps, 1)


def back_and_forth(motor, n):
    for i in range(0, n):
        motor.steps_opperate(1000, 1+(-1)**i)



def main():
    stor_motor = pi5modules.Big_Stepper_Motor(23, 24, "Motor1", 0)
    liten_motor = pi5modules.Small_Stepper_Motor(20, 21, "Motor2", 0)


    # DET ER IKKE SATT EN MAKS GRENSE FOR STEG PÅ MOTORNE
    forwards(stor_motor, 400)
    backwards(stor_motor, 400)
    back_and_forth(stor_motor, 2)


if __name__ == "__main__":
    main()
