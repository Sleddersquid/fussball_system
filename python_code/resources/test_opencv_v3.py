import numpy as np
import cv2

stream = cv2.VideoCapture(0)

while(True):
    ret, frame = stream.read()
    
    if not ret:
        print("no more stream :(")
        break
    