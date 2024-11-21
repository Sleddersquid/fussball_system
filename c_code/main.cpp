#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <tgmath.h>

// For the threads
#include <thread>
// #include <mutex>
#include <condition_variable>
#include <chrono>

// For GPIO, OpenCV and lccv (enables opencv to use the picamera v3)
#include <gpiod.hpp>
#include <opencv2/opencv.hpp>
#include <lccv.hpp>

// For the motors and the threadsafe deque
#include "library/motors.cpp"
// #include "library/motors.hpp"
#include "library/threadsafe_containers/queue-threadsafe.hpp"

#define PI 3.14159265

// #define MAX_LEN_DEQUE 32

// avg FPS at HD is 125 without computation of center, with center computing, 111.111, and at FHD is 48.478
#define CAMERA_HEIGHT 720    // Can be SD: 480, HD: 720, FHD: 1080, QHD: 1440
#define CAMERA_WIDTH 1280    // Can be SD: 640, HD: 1280, FHD: 1920, QHD: 2560
#define CAMERA_FRAMERATE 200 // If fps higher than what the thread can handle, it will just run lower fps.

#define SECONDS_ACTIVE 3000

// ------------------------ TODO: ------------------------ //
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
cv::Point calculateCenter(const std::vector<cv::Point> &contour)
{
    cv::Moments M = cv::moments(contour);
    if (M.m00 != 0)
    {
        // See https://docs.opencv.org/3.4/d8/d23/classcv_1_1Moments.html for center (x¯, y¯)
        return cv::Point(static_cast<int>(M.m10 / M.m00), static_cast<int>(M.m01 / M.m00));
    }
    return cv::Point(0, 0); // Return a dummy value if contour area is zero
}

long long getTimestamp()
{
    const std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    const std::chrono::microseconds epoch = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
    return epoch.count();
}

cv::Point simplePredict(cv::Point &old_ball_pos, cv::Point &new_ball_pos) {
    if (old_ball_pos == cv::Point(-99, -99)) {
        return;
    }
    new_ball_pos = cv::Point(new_ball_pos.x + (new_ball_pos.x - old_ball_pos.x), new_ball_pos.y);
    return new_ball_pos;
} 



// --------------- Func for the threads --------------- //
void opencv(std::threadsafe::queue<cv::Point> &deque_ball_pos)
{
    bool running = true;

    // int framesNumber = 0;
    // long long startTime = getTimestamp();
    // long long totalTime = 0;

    std::cout << "Sample program for LCCV video capture" << std::endl;
    std::cout << "Press ESC to stop. (Does not work if no window is displayed)" << std::endl;

    cv::Mat image, mask, HSV;
    lccv::PiCamera cam;

    cv::Scalar hsv_lower(0, 120, 120);
    cv::Scalar hsv_upper(10, 255, 255);

    cam.options->video_width = CAMERA_WIDTH;
    cam.options->video_height = CAMERA_HEIGHT;
    cam.options->framerate = CAMERA_FRAMERATE;
    cam.options->verbose = true;
    // cam.options->list_cameras = true;

    // cv::namedWindow("Video", cv::WINDOW_NORMAL);
    // cv::namedWindow("Mask", cv::WINDOW_NORMAL);
    cam.startVideo();

    // int framesNumber = 0;
    // long long startTime = getTimestamp();
    // long long totalTime = 0;

    cv::Point new_center(0, 0);
    cv::Point old_center(0, 0);

    auto thread_start_time = std::chrono::high_resolution_clock::now();
    while (running)
    {
        if (!cam.getVideoFrame(image, 1000))
        {
            std::cerr << "Timeout error" << std::endl;
        }
        else
        {
            cv::cvtColor(image, HSV, cv::COLOR_BGR2HSV);

            cv::inRange(HSV, hsv_lower, hsv_upper, mask);

            cv::erode(mask, mask, cv::Mat(), cv::Point(-1, -1), 3);
            cv::dilate(mask, mask, cv::Mat(), cv::Point(-1, -1), 3);

            // Find contours in the mask
            std::vector<std::vector<cv::Point>> contours;
            cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);


            if (!contours.empty()) {
                // Find the largest contour
                std::vector<cv::Point> largestContour = *std::max_element(contours.begin(), contours.end(), 
                [](const std::vector<cv::Point> &a, const std::vector<cv::Point> &b) {
                    return cv::contourArea(a) < cv::contourArea(b);
                });
                new_center = calculateCenter(largestContour);
            }


            // Only send instruction if the motors needs to be moved.
            if (new_center.x == 0 && new_center.y == 0) {
                continue;
            }

            if (abs(new_center.x - old_center.x) < 2) { // && (new_ball_pos.y < 200)
                old_center = new_center;
                continue;
            }

            deque_ball_pos.push(new_center);





            
            old_center = new_center;
        }
        if ((std::chrono::high_resolution_clock::now() - thread_start_time) > std::chrono::seconds{SECONDS_ACTIVE + 1})
        {
            running = false;
        }
    }

    cam.stopVideo();
    // cv::destroyWindow("Video");
    // cv::destroyWindow("Mask");
    cv::destroyAllWindows();
    deque_ball_pos.push(cv::Point(0, 0)); // Send a dummy value to stop the fussball thread
    std::cout << "End of opencv" << std::endl;
}

// The multiple params may need to be adjusted
void fussball_system(std::threadsafe::queue<cv::Point> &deque_ball_pos, Big_Stepper_motor &big_motor_1, Small_Stepper_motor &small_motor_1)
{
    bool running = true;

    int theta;
    cv::Point ball_pos;

    std::thread thread3;

    int direction = 0;

    auto thread_start_time = std::chrono::high_resolution_clock::now();
    while (running) {
        // if (!deque_ball_pos.try_top(new_ball_pos)) {
        //     std::cout << "No new ball pos" << std::endl;
        //     big_motor_1.reset();
        //     small_motor_1.reset();
        //     continue;
        // }

        // if (deque_ball_pos.empty()) {
        //     // std::cout << "No new ball pos" << std::endl;
        //     continue;
        // }

        // deque_ball_pos.try_pop(new_ball_pos);
        deque_ball_pos.wait_pop(ball_pos);

        // std::cout << "x: " << new_ball_pos.x << " y: " << new_ball_pos.y << std::endl;

        // If there isn't a significant enough change in the x value, don't opperate the motor

        // try {
        big_motor_1.go_to_coord(ball_pos.x);

        // if (new_ball_pos.y > 450) {
        //     small_motor_1.go_to_angle(-45);
        // } else {
        //     small_motor_1.go_to_angle(0);
        // }
        // }
        // catch (const std::exception &e) {
        //     std::cerr << e.what() << std::endl;
        //     break;
        // }

        // std::cout << "x: " << old_ball_pos.x << " y: " << old_ball_pos.y << std::endl;

        // If is directly infrot of the goal, x = x
        
        // direction = !direction;

        if ((std::chrono::high_resolution_clock::now() - thread_start_time) > std::chrono::seconds{SECONDS_ACTIVE})
        {
            running = false;
        }
    }

    std::cout << "End of fussball" << std::endl;
}

// ------------------ MAIN VARIABLES ------------------ //

gpiod::chip chip("gpiochip0");

// Created in this scope so the
Big_Stepper_motor big_motor_1(23, 24, chip);
Small_Stepper_motor small_motor_1(20, 21, chip);

// The mutex is in the threadsafe queue
std::threadsafe::queue<cv::Point> deque_ball_pos;

// Create the threads
std::thread thread1;
std::thread thread2;


// ------------------ MAIN FUNCTION ------------------ //
int main()
{

    try
    {
        thread1 = std::thread(opencv, std::ref(deque_ball_pos));
        thread2 = std::thread(fussball_system, std::ref(deque_ball_pos), std::ref(big_motor_1), std::ref(small_motor_1));
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    // std::this_thread::sleep_for(std::chrono::seconds(SECONDS_ACTIVE + 1));

    // Join the threads. Will wait here until it is joinable (wait_pop of the deque is hindering this )
    thread1.join();
    std::cout << "Closed thread 1" << std::endl;

    // Release the lines. Is it the opencv or the fussball that dosne't stop completly?
    big_motor_1.reset();
    small_motor_1.reset();

    thread2.join();
    std::cout << "Closed thread 2" << std::endl;
    // std::cout << "Threads closed" << std::endl;

    // There looks like there is no need to close the gpio chip.
    // Only need to release the lines, and that is done in the destructor of the motor class
    std::cout << "End of file" << std::endl;
    return 0;
}
