#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <tgmath.h> // For arctan and general math

// For the threads
#include <thread>
#include <condition_variable>
#include <chrono>

// For GPIO, OpenCV and lccv(enables opencv to use the picamera v3)
#include <gpiod.hpp>
#include <opencv2/opencv.hpp>
#include <lccv.hpp>

// For the motors and the threadsafe deque
#include "library/motors.cpp" // It is not supposed to be like this? Why. I'll figure it out later.
// #include "library/motors.hpp"
#include "library/threadsafe_containers/queue-threadsafe.hpp"

// avg FPS at HD is 125 without computation of center, with center computing, 111.111
// When system is running with all functions, might be lowered down to 100-90 fps. 
#define CAMERA_HEIGHT 720    // Can be SD: 480, HD: 720, FHD: 1080, QHD: 1440
#define CAMERA_WIDTH 1280    // Can be SD: 640, HD: 1280, FHD: 1920, QHD: 2560
#define CAMERA_FRAMERATE 200 // If fps higher than what the thread can handle, it will just run lower fps.

#define SECONDS_ACTIVE 600

// ------------------------ TODO: ------------------------ //

/**
 * @brief Function to calculate the center of a contour using moments
 * @param contour - The contour to calculate the center of
 * @return The center of the contour as cv::Point
 */
cv::Point calculateCenter(const std::vector<cv::Point> &contour) {
    cv::Moments M = cv::moments(contour);
    if (M.m00 != 0) {
        // See https://docs.opencv.org/3.4/d8/d23/classcv_1_1Moments.html for center of mass (x¯, y¯)
        return cv::Point(static_cast<int>(M.m10 / M.m00), static_cast<int>(M.m01 / M.m00));
    }
    return cv::Point(0, 0); // If no center is found, return (0, 0)
}

long long getTimestamp() {
    const std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    const std::chrono::microseconds epoch = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
    return epoch.count();
}

cv::Point simplePredict(cv::Point old_ball_pos, cv::Point new_ball_pos, int magnitude = 5) {
    return cv::Point(new_ball_pos.x + magnitude*(new_ball_pos.x - old_ball_pos.x), new_ball_pos.y + magnitude*(new_ball_pos.y - old_ball_pos.y));
}

cv::Point intersect_determinant(cv::Point p1, cv::Point p2, cv::Point p3, cv::Point p4) {
    // Handle if the lower determnant is zero
    float det = ((p1.x - p2.x)*(p3.y - p4.y) - (p1.y - p2.y)*(p3.x - p4.x));
    if (det == 0) {
        return cv::Point(0, 0);
    }
    // See https://en.wikipedia.org/wiki/Line–line_intersection for formula
    // Will prob drop the px, just needed for visualisation
    float px = ((p1.x*p2.y - p1.y*p2.x)*(p3.x - p4.x) - (p1.x - p2.x)*(p3.x*p4.y - p3.y*p4.x))/det;
    float py = ((p1.x*p2.y - p1.y*p2.x)*(p3.y - p4.y) - (p1.y - p2.y)*(p3.x*p4.y - p3.y*p4.x))/det;
    return cv::Point(px, py);
}

// --------------- Func for the threads --------------- //
void opencv(std::threadsafe::queue<cv::Point> &deque_ball_pos) {
    bool running = true;


    std::cout << "Press ESC to stop. (Does not work if no window is displayed)" << std::endl;

    cv::Mat image, mask, HSV;
    lccv::PiCamera cam;

    // Was (0, 120, 120) and (10, 255, 255).
    // Lightings conditions such as sunlight might detect hands and face 
    cv::Scalar hsv_lower(0, 150, 50); // 0, 150, 50
    cv::Scalar hsv_upper(15, 255, 255);  // 15, 255, 255

    cam.options->video_width = CAMERA_WIDTH;
    cam.options->video_height = CAMERA_HEIGHT;
    cam.options->framerate = CAMERA_FRAMERATE;
    cam.options->verbose = true;
    // cam.options->list_cameras = true;

    // cv::namedWindow("Video", cv::WINDOW_NORMAL);
    // cv::namedWindow("Mask", cv::WINDOW_NORMAL);
    cam.startVideo();

    cv::Point new_center(0, 0);
    cv::Point old_center(0, 0);

    cv::Point ball_intersect(0, 0);
    cv::Point predictball_pos(0, 0);

    auto thread_start_time = std::chrono::high_resolution_clock::now();
    while (running) {
        if (!cam.getVideoFrame(image, 1000)) {
            std::cerr << "Timeout error" << std::endl;
        } else {
            cv::cvtColor(image, HSV, cv::COLOR_BGR2HSV);

            cv::inRange(HSV, hsv_lower, hsv_upper, mask);

            cv::erode(mask, mask, cv::Mat(), cv::Point(-1, -1), 3);
            cv::dilate(mask, mask, cv::Mat(), cv::Point(-1, -1), 3);

            std::vector<std::vector<cv::Point>> contours;
            cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

            if (!contours.empty()) {
                std::vector<cv::Point> largestContour = *std::max_element(contours.begin(), contours.end(), 
                [](const std::vector<cv::Point> &a, const std::vector<cv::Point> &b) {
                    return cv::contourArea(a) < cv::contourArea(b);
                }); // Lambda function. Finds the largest contour
                new_center = calculateCenter(largestContour);
            }

            if (new_center.x == 0 && new_center.y == 0) {
                continue;
            }

            if (abs(new_center.x - old_center.x) < 3 || abs(new_center.y - old_center.y) < 3) {
                old_center = new_center;
                continue;
            }

            predictball_pos = simplePredict(old_center, new_center);

            if (old_center.y - new_center.y < 1) { // If the ball is moving towards the goal
                ball_intersect = intersect_determinant(cv::Point(85, 760), cv::Point(1167, 709), old_center, predictball_pos);
            } else {
                ball_intersect = cv::Point(643, 734); // Reset?
            }

            // Only add intersect point if it is going into the goal
            if (ball_intersect.x > 455 && ball_intersect.x < 830) {
                deque_ball_pos.push(ball_intersect);
                // std::cout << "Intersect: " << "x: " << ball_intersect.x << " y: " << ball_intersect.y << std::endl;
                cv::line(image, ball_intersect, new_center, cv::Scalar(0, 0, 255), 15);
            } else {
                // std::cout << "No ball intersection" << std::endl;
            }

            cv::imshow("Video", image);
            cv::imshow("Mask", mask);

            // show image with the tracked object
            // quit on q button
            if (cv::waitKey(1) == 'q') {
                break;
            }

            old_center = new_center;

            if ((std::chrono::high_resolution_clock::now() - thread_start_time) > std::chrono::seconds{SECONDS_ACTIVE + 1}) {
                running = false;
            }
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
void fussball_system(std::threadsafe::queue<cv::Point> &deque_ball_pos, Big_Stepper_motor &big_motor_1, Small_Stepper_motor &small_motor_1) {
    bool running = true;

    // int theta;
    cv::Point old_intersect(0, 0);
    // cv::Point new_ball(0, 0);

    cv::Point intersect_point(0, 0);

    int direction = 0;

    auto thread_start_time = std::chrono::high_resolution_clock::now();
    while (running) {

        // deque_ball_pos.try_pop(new_ball_pos);
        deque_ball_pos.wait_pop(intersect_point);

        if (abs(old_intersect.x - intersect_point.x) > 20) { // If two inerections are far enough apart, then move the goal keeper
            big_motor_1.go_to_coord(intersect_point.x);
        }

        old_intersect = intersect_point;

        if ((std::chrono::high_resolution_clock::now() - thread_start_time) > std::chrono::seconds{SECONDS_ACTIVE}) {
            running = false;
        }
    }

    std::cout << "End of fussball" << std::endl;
}

// ------------------ MAIN VARIABLES ------------------ //

gpiod::chip chip("gpiochip0");

// Created in this scope so that it is possible to reset the motors.
// The problem is the deque waiting.  
Big_Stepper_motor big_motor_1(23, 24, chip);
Small_Stepper_motor small_motor_1(20, 21, chip);

// The mutex is in the threadsafe queue
std::threadsafe::queue<cv::Point> deque_ball_pos;

// Create the threads
std::thread thread1;
std::thread thread2;

// ------------------ MAIN FUNCTION ------------------ //
int main() {

    try {
        thread1 = std::thread(opencv, std::ref(deque_ball_pos));
        thread2 = std::thread(fussball_system, std::ref(deque_ball_pos), std::ref(big_motor_1), std::ref(small_motor_1));
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    // Join the thread1 (opencv)
    thread1.join();
    std::cout << "Closed thread 1" << std::endl;

    // Release the lines. Is it the opencv or the fussball that dosne't stop completly?
    big_motor_1.reset();
    small_motor_1.reset();

    // Join the thread2 (fussball_system) Will wait here until it is joinable (wait_pop of the deque is hindering this )
    thread2.join();
    std::cout << "Closed thread 2" << std::endl;

    // There looks like there is no need to close the gpio chip.
    // Only need to release all the lines, and that is done in the destructor of the motor class
    std::cout << "End of file" << std::endl;
    return 0;
}
