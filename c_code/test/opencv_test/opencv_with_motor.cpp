#include <opencv2/opencv.hpp>
#include <iostream>

#include <unistd.h>
#include <gpiod.hpp>

// Absolute path. Should be reviced under revision
#include "include_cpp_file/motors.cpp"
#include "library/deque_extra.hpp"

gpiod::chip chip("gpiochip0");

// This needs to be better organized in the future.
int big_motor1_pulse_pin = 20;
int big_motor1_dir_pin = 21;    

int small_motor1_pulse_pin = 23;
int small_motor1_dir_pin = 24;

// Open the GPIO chip
// Connect to the GPIO line
Big_Stepper_motor big_motor1(big_motor1_pulse_pin, big_motor1_dir_pin, chip, 0);
Small_Stepper_motor small_motor1(small_motor1_pulse_pin, small_motor1_dir_pin, chip, 0);

// OPENCV ER CHAT GPT KODE. 

// Function to calculate the center of a contour using moments
cv::Point calculateCenter(const std::vector<cv::Point>& contour) {
    cv::Moments M = cv::moments(contour);
    if (M.m00 != 0) {
        // See https://docs.opencv.org/3.4/d8/d23/classcv_1_1Moments.html for center (x¯, y¯)
        // 
        return cv::Point(static_cast<int>(M.m10 / M.m00), static_cast<int>(M.m01 / M.m00));
    } else {
        return cv::Point(0, 0); // Return a dummy value if contour area is zero
    }
}

int main() {
    // Define lower and upper boundaries for the "green" ball in HSV color space
    cv::Scalar greenLower(18, 100, 204);
    cv::Scalar greenUpper(26, 100, 204);

    // Deque to store tracked points (32 maximum points)
    std::deque<cv::Point> pts;
    const int maxPoints = 32;

    // Open the default camera
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open the video stream." << std::endl;
        return -1;
    }

    // Allow the camera to warm up
    cv::waitKey(2000);

    // Main loop
    while (true) {
        cv::Mat frame;
        cap >> frame; // Capture the current frame
        if (frame.empty()) {
            std::cerr << "Error: Could not capture frame." << std::endl;
            break;
        }

        // Resize the frame
        cv::resize(frame, frame, cv::Size(600, 600));

        // Blur the frame and convert it to HSV color space
        cv::Mat blurred, hsv;
        cv::GaussianBlur(frame, blurred, cv::Size(11, 11), 0);
        cv::cvtColor(blurred, hsv, cv::COLOR_BGR2HSV);

        // Create a mask for the color "green"
        cv::Mat mask;
        cv::inRange(hsv, greenLower, greenUpper, mask);

        // Erode and dilate to remove small blobs
        cv::erode(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);
        cv::dilate(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);

        // Find contours in the mask
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        cv::Point center(0, 0);
        if (!contours.empty()) {
            // Find the largest contour
            std::vector<cv::Point> largestContour = *std::max_element(contours.begin(), contours.end(),
                [](const std::vector<cv::Point>& a, const std::vector<cv::Point>& b) {
                    return cv::contourArea(a) < cv::contourArea(b);
                });

            // Compute the minimum enclosing circle and center
            float radius;
            cv::Point2f enclosingCenter;
            cv::minEnclosingCircle(largestContour, enclosingCenter, radius);

            // Calculate the centroid (center of mass)
            center = calculateCenter(largestContour);

            // Only proceed if the radius meets a minimum size
            if (radius > 10) {
                // Draw the circle and the centroid on the frame TODO: REMOVE THIS
                cv::circle(frame, enclosingCenter, static_cast<int>(radius), cv::Scalar(0, 255, 255), 2);
                cv::circle(frame, center, 5, cv::Scalar(0, 0, 255), -1);
            }
        }

        // Update the deque with the new center
        pts.push_front(center);
        if (pts.size() > maxPoints) {
            pts.pop_back();
        }

        // Optionally draw connecting lines between points
        for (size_t i = 1; i < pts.size(); ++i) {
            if (pts[i - 1] == cv::Point(0, 0) || pts[i] == cv::Point(0, 0)) {
                continue;
            }
            int thickness = static_cast<int>(sqrt(32.0 / (i + 1)) * 2.5);
            cv::line(frame, pts[i - 1], pts[i], cv::Scalar(0, 0, 255), thickness);
        }

        // Show the frame and the mask
        cv::imshow("Frame", frame);
        cv::imshow("Mask", mask);

        // Check if 'q' was pressed to exit the loop
        char key = static_cast<char>(cv::waitKey(1));
        if (key == 'q') {
            break;
        }

        // Add a short sleep (simulating time.sleep(0.1) in Python)
        cv::waitKey(100);
    }

    // Release the video stream and close windows
    cap.release();
    cv::destroyAllWindows();
    return 0;
}