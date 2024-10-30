#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <tgmath.h>

// For the threads and such
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

// For GPIO and OpenCV
#include <gpiod.hpp>
#include <opencv2/opencv.hpp>
#include <lccv.hpp>

// For the motors and the deque
#include "include_cpp_file/motors.cpp"
// #include "library/motors.hpp"
// #include "library/deque_extra.hpp"
#include "library/threadsafe_containers/queue-threadsafe.hpp"
// #include "library/Row.hpp"

#define PI 3.14159265

#define MAX_LEN_DEQUE 32

// avg FPS at HD is 125 without computhon center, with center computing, 111.111, and at FHD is 48.478 
#define CAMERA_HEIGHT 720       // Can be SD: 480, HD: 720, FHD: 1080, QHD: 1440
#define CAMERA_WIDTH 1280       // Can be SD: 640, HD: 1280, FHD: 1920, QHD: 2560
#define CAMERA_FRAMERATE 101    // If fps higher than what the thread can handle, it will just run lower fps.

#define SECONDS_ACTIVE 2

// ------------------------ TODO: ------------------------ //
// 1 - Make an algorithm that chooses what row and another alogrithm for the players


// ------------------ GLOBAL VARIABLES ------------------ //

// Opening the GPIO chip
// The chip dosen't need to be closed/resources freed, it does so automatically when all the lines are dropped/released.
// gpiod::chip chip("gpiochip0");

// ------------------ INIT OF MOTORS ------------------ //
// All the pins are numbered after the GPIO pins, and not the lines

// First row -- Initialized inside the thread
// Big_Stepper_motor big_motor_row0(23, 24, chip, 0);
// Small_Stepper_motor small_motor_row0(20, 21, chip, 0);

// // Second row
// Big_Stepper_motor big_motor_row1(2, 3, chip, 1);
// Small_Stepper_motor small_motor_row1(14, 15, chip, 1);

// // Third row
// Big_Stepper_motor big_motor_row2(17, 18, chip, 2);
// Small_Stepper_motor small_motor_row2(4, 5, chip, 2);

// // Fourth row
// Big_Stepper_motor big_motor_row3(12, 13, chip, 3);
// Small_Stepper_motor small_motor_row3(10, 11, chip, 3);



// ------------------ INIT OF ROWS ------------------ //

// ------------------ HELPER FUNCTIONS ------------------ //

/**
 * @brief Function to calculate the center of a contour using moments
 * @param contour - The contour to calculate the center of
 * @return The center of the contour as cv::Point
 */
cv::Point calculateCenter(const std::vector<cv::Point> &contour) {
    cv::Moments M = cv::moments(contour);
    if (M.m00 != 0) {
        // See https://docs.opencv.org/3.4/d8/d23/classcv_1_1Moments.html for center (x¯, y¯)
        return cv::Point(static_cast<int>(M.m10 / M.m00), static_cast<int>(M.m01 / M.m00));
    }
    return cv::Point(0, 0); // Return a dummy value if contour area is zero
}

long long getTimestamp()
{
    const std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    const std::chrono::microseconds epoch = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
    return  epoch.count();
}

// --------------- Func for the threads --------------- //
void opencv(std::threadsafe::queue<cv::Point> &deque_ball_pos) {
    bool running = true;

    int framesNumber = 0;
    long long startTime = getTimestamp();
    long long totalTime = 0;

    std::cout << "Sample program for LCCV video capture" << std::endl;
    std::cout << "Press ESC to stop. (Does not work if no window is displayed)" << std::endl;

    cv::Mat image, mask, HSV;
    lccv::PiCamera cam;

    cv::Scalar hsv_l(0, 120, 120);
    cv::Scalar hsv_h(10, 255, 255);

    cam.options->video_width = CAMERA_WIDTH;
    cam.options->video_height = CAMERA_HEIGHT;
    cam.options->framerate = CAMERA_FRAMERATE;
    cam.options->verbose = true;
    cam.options->list_cameras = true;

    // cv::namedWindow("Video", cv::WINDOW_NORMAL);
    // cv::namedWindow("Mask", cv::WINDOW_NORMAL);
    cam.startVideo();
    int ch = 0;

    auto thread_start_time = std::chrono::high_resolution_clock::now();
    while (running) {
        if (!cam.getVideoFrame(image, 1000)) {
            std::cerr << "Timeout error" << std::endl;
        }
        else {
            cv::cvtColor(image, HSV, cv::COLOR_BGR2HSV);

            cv::inRange(HSV, hsv_l, hsv_h, mask);

            cv::erode(mask, mask, cv::Mat(), cv::Point(-1, -1), 1);
            cv::dilate(mask, mask, cv::Mat(), cv::Point(-1, -1), 1);

            // Find contours in the mask
            std::vector<std::vector<cv::Point>> contours;
            cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

            cv::Point center(0, 0);
            if (!contours.empty()) {
                // Find the largest contour
                std::vector<cv::Point> largestContour = *std::max_element(contours.begin(), contours.end(),
                [](const std::vector<cv::Point> &a, const std::vector<cv::Point> &b) {
                    return cv::contourArea(a) < cv::contourArea(b);
                });

                // Compute the minimum enclosing circle and center
                float radius;
                cv::Point2f enclosingCenter;
                cv::minEnclosingCircle(largestContour, enclosingCenter, radius);

                // Calculate the centroid (center of mass)
                center = calculateCenter(largestContour);

                // // Only proceed if the radius meets a minimum size
                // if (radius > 10) {
                //     // Draw the circle and the centroid on the frame TODO: REMOVE THIS
                //     cv::circle(image, enclosingCenter, static_cast<int>(radius), cv::Scalar(0, 255, 255), 2);
                //     cv::circle(image, center, 5, cv::Scalar(0, 0, 255), -1);
                // }
            }

            deque_ball_pos.push(center);

            framesNumber++;

            // ----------------- For drawing the path of the ball ----------------- //
            // for (size_t i = 1; i < deque_ball_pos.size(); ++i) {
            //     if (deque_ball_pos[i - 1] == cv::Point(0, 0) || deque_ball_pos[i] == cv::Point(0, 0)) {
            //         continue;
            //     }
            //     int thickness = static_cast<int>(sqrt(32.0 / (i + 1)) * 2.5);
            //     cv::line(image, deque_ball_pos[i - 1], deque_ball_pos[i], cv::Scalar(0, 0, 255), thickness);
            // }

            // cv::imshow("Video", image);
            // cv::imshow("Mask", mask);

        }
        if ((std::chrono::high_resolution_clock::now() - thread_start_time) > std::chrono::seconds{SECONDS_ACTIVE}) {
            running = false;
        }
    }

    totalTime = getTimestamp();
    float avgFPS = (totalTime -startTime)/ 1000 / framesNumber;
    std::cout << "Average FPS: " << (1000 / avgFPS) << std::endl;
    
    cam.stopVideo();
    // cv::destroyWindow("Video");
    // cv::destroyWindow("Mask");
    std::cout << "End of opencv" << std::endl;
}

// The multiple params may need to be adjusted
void fussball_system(std::threadsafe::queue<cv::Point> &deque_ball_pos)
{
    bool running = true;

    gpiod::chip chip("gpiochip0");

    Big_Stepper_motor big_motor_row0(23, 24, chip, 0);
    Small_Stepper_motor small_motor_row0(20, 21, chip, 0);

    int theta;
    cv::Point ball_pos;

    auto thread_start_time = std::chrono::high_resolution_clock::now();
    while (running)
    {
        // This needs a lock
        // mtx.native_handle
        deque_ball_pos.wait_pop(ball_pos);


        if (ball_pos.x == 0 and ball_pos.y == 0) {
            continue;
        }

        // deque_ball_pos.

        // theta = atan2(ball_pos.y - CAMERA_HEIGHT/2, ball_pos.x - CAMERA_WIDTH/2) * (180 / PI);

        // small_motor_row0.go_to_angle(theta);

        std::cout << "x: " << ball_pos.x << " y: " << ball_pos.y << std::endl;

        if ((std::chrono::high_resolution_clock::now() - thread_start_time) > std::chrono::seconds{SECONDS_ACTIVE}) {
            running = false;
        }
    }
    std::cout << "End of fussball" << std::endl;
}

// void fussball_single_motor(cv::Point ball_pos, Small_Stepper_motor &motor)
// {
//     int theta;
//     theta = atan2(ball_pos.y - CAMERA_HEIGHT/2, ball_pos.x - CAMERA_WIDTH/2) * (180 / PI);

//     motor.go_to_angle(theta);
// }

// ------------------ MAIN VARIABLES ------------------ //

// std::atomic<std::array<cv::Point, 3>> deque_ball_pos;
// The mutex is in the threadsafe queue
std::threadsafe::queue<cv::Point> deque_ball_pos;

// Create the threads
// std::thread thread1;
// std::thread thread2;

int a = 0;
// ------------------ MAIN FUNCTION ------------------ //
int main()
{

    try {
        std::thread thread1 = std::thread(opencv, std::ref(deque_ball_pos));
        std::thread thread2 = std::thread(fussball_system, std::ref(deque_ball_pos));

    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(SECONDS_ACTIVE + 1));

    // Join the threads
    std::cout << "Closing the threads" << std::endl;
    thread1.join();
    thread2.join();
    std::cout << "Threads closed" << std::endl;

    // There looks like there is no need to close the gpio chip.
    // Only need to release the lines, and that is done in the destructor of the motor class
    std::cout << "End of file" << std::endl;
    return 0;
}
