# https://github.com/freedomwebtech/colortracking/blob/main/track.py
# https://www.geeksforgeeks.org/python-opencv-namedwindow-function/

import cv2 as cv
from imutils.video import VideoStream
from imutils import resize
import time


# vs = VideoStream(src=1).start()

cv.namedWindow("Trackbar Window", cv.WINDOW_AUTOSIZE)
# Lower HSV and Upper HSV. Range for HSV in python is (0-179, 0-255, 0-255)
# Using lambda because it needs a callable function. In this case it dosen't need to do anything
cv.createTrackbar("Lower H", "Trackbar Window", 0, 179, lambda a: ())
cv.createTrackbar("Lower S", "Trackbar Window", 0, 255, lambda a: ())
cv.createTrackbar("Lower V", "Trackbar Window", 0, 255, lambda a: ())
cv.createTrackbar("Upper H", "Trackbar Window", 179, 179, lambda a: ())
cv.createTrackbar("Upper S", "Trackbar Window", 255, 255, lambda a: ())
cv.createTrackbar("Upper V", "Trackbar Window", 255, 255, lambda a: ())

# try:
    # global vs
vs = VideoStream(src=1).start()
# except:
    # vs = VideoStream(src=0).start()


time.sleep(2)

while True:
    frame = vs.read()
    frame = resize(frame, width=600, height=600)
    hsv = cv.cvtColor(frame, cv.COLOR_BGR2HSV)
    
    l_h = cv.getTrackbarPos("Lower H", "Trackbar Window")
    l_s = cv.getTrackbarPos("Lower S", "Trackbar Window")
    l_v = cv.getTrackbarPos("Lower V", "Trackbar Window")
    u_h = cv.getTrackbarPos("Upper H", "Trackbar Window")
    u_s = cv.getTrackbarPos("Upper S", "Trackbar Window")
    u_v = cv.getTrackbarPos("Upper V", "Trackbar Window")
    lower_blue = (l_h, l_s, l_v)
    upper_blue = (u_h, u_s, u_v)
    
    
    mask = cv.inRange(hsv, lower_blue, upper_blue)
    result = cv.bitwise_and(frame, frame, mask=mask)    

    # show thresholded image
    cv.imshow("mask", mask)
    cv.imshow("result", result)  

    key = cv.waitKey(1) & 0xFF
    if key == ord("q"):
        break
    
vs.release()
cv.destroyAllWindows()
