#include <iostream>
#include <array>
#include <vector>
#include <math.h>


// For the threads and such
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

// For GPIO and OpenCV
#include <gpiod.hpp>
#include <opencv2/opencv.hpp>

// For the motors and the deque
#include "include_cpp_file/motors.cpp"
#include "library/deque_extra.hpp"
#include "library/Row.hpp"

#define PI 3.14159265

// ------------------------ TODO: ------------------------ //
// 1 - Make an algorithm that chooses what row and another alogrithm for the players

// ------------------ GLOBAL VARIABLES ------------------ //

// Opening the GPIO chip
// The chip dosen't need to be closed/resources freed, it does so automatically when all the lines are dropped/released.
gpiod::chip chip("gpiochip0");

struct center_point {
    int x = 300;
    int y = 300;
};




// ------------------ INIT OF MOTORS ------------------ //
// All the pins are numbered after the GPIO pins, and not the lines

// First row
Big_Stepper_motor big_motor_row0(23, 24, chip, 0);
Small_Stepper_motor small_motor_row0(20, 21, chip, 0);

// Second row
Big_Stepper_motor big_motor_row1(2, 3, chip, 1);
Small_Stepper_motor small_motor_row1(14, 15, chip, 1);

// Third row
Big_Stepper_motor big_motor_row2(17, 18, chip, 2);
Small_Stepper_motor small_motor_row2(4, 5, chip, 2);

// Fourth row
Big_Stepper_motor big_motor_row3(12, 13, chip, 3);
Small_Stepper_motor small_motor_row3(10, 11, chip, 3);

// ------------------ INIT OF ROWS ------------------ //

// ------------------ HELPER FUNCTIONS ------------------ //

/**
 * @brief Function to calculate the center of a contour using moments
 * @param contour - The contour to calculate the center of
 * @return The center of the contour as cv::Point
 */
cv::Point calculateCenter(const std::vector<cv::Point> &contour)
{
    cv::Moments M = cv::moments(contour);
    if (M.m00 != 0)
    {
        // See https://docs.opencv.org/3.4/d8/d23/classcv_1_1Moments.html for center (x¯, y¯)
        return cv::Point(static_cast<int>(M.m10 / M.m00), static_cast<int>(M.m01 / M.m00));
    }
    else
    {
        return cv::Point(0, 0); // Return a dummy value if contour area is zero
    }
}

// --------------- Func for the threads --------------- //

void opencv(max_deque<cv::Point_<int>, 32> &ball_position, std::mutex &mtx) {

    while (true) {
        /* code */
    }
}

void fussball_system(max_deque<cv::Point_<int>, 32> &ball_position, std::mutex &mtx) {
    int theta;
    cv::Point_<int> ball_pos;

    while (true) {
        // This needs a lock
        mtx.lock();
        ball_pos = ball_position.back();
        mtx.unlock();
        // Calculate the angle of position of the ball
        // Since atan gives radians, convert it to degrees
        theta = atan2(ball_pos.y - 300, ball_pos.x - 300) * (180 / PI);

        small_motor_row0.go_to_angle(theta);
    }
}

int main() {

    max_deque<cv::Point_<int>, 32> ball_position;

    // Initialization of mutex
    std::mutex mtx;

    // Create the threads
    std::thread thread1;
    std::thread thread2;

    
    try {
        thread1 = std::thread{opencv, std::ref(ball_position), std::ref(mtx)};
        thread2 = std::thread{fussball_system, std::ref(ball_position), std::ref(mtx)};
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    // Join the threads
    thread1.join();
    thread2.join();

    // There looks like there is no need to close the gpio chip.
    // Only need to release the lines, and that is done in the destructor of the motor class
    std::cout << "End of file" << std::endl;
    return 0;
}
