#include <iostream>
#include <vector>

// For the threads and such
#include <chrono>

// For GPIO and OpenCV
#include <opencv2/opencv.hpp>
#include <lccv.hpp>

// For the Motors, Rows and The deque

#define PI 3.14159265

// avg FPS at HD is 111.111, and at FHD is 48.478 
#define CAMERA_HEIGHT 720       // Can be SD: 480, HD: 720, FHD: 1080, QHD: 1440
#define CAMERA_WIDTH 1280       // Can be SD: 640, HD: 1280, FHD: 1920, QHD: 2560
#define CAMERA_FRAMERATE 120    // If fps higher than what the thread maneges, it will just run lower fps.

// ------------------------ TODO: ------------------------ //
// 1 - Make an algorithm that chooses what row and another alogrithm for the players

// ------------------ GLOBAL VARIABLES ------------------ //

/**
 * @brief Function to calculate the center of a contour using moments
 * @param contour - The contour to calculate the center of
 * @return The center of the contour as cv::Point
 */
cv::Point calculateCenter(const std::vector<cv::Point> &contour)
{
    cv::Moments M = cv::moments(contour);
    if (M.m00 != 0) {
        // See https://docs.opencv.org/3.4/d8/d23/classcv_1_1Moments.html for center (x¯, y¯)
        return cv::Point(static_cast<int>(M.m10 / M.m00), static_cast<int>(M.m01 / M.m00));
    }
    else {
        return cv::Point(0, 0); // Return a dummy value if contour area is zero
    }
}

cv::Mat image, mask, HSV;
lccv::PiCamera cam;

cv::Scalar hsv_l(0, 120, 120);
cv::Scalar hsv_h(10, 255, 255);

// ------------------ MAIN FUNCTION ------------------ //
int main()
{
    std::cout << "Sample program for LCCV video capture" << std::endl;
    std::cout << "Press ESC to stop. (Does not work if no window is displayed)" << std::endl;

    cam.options->video_width = CAMERA_WIDTH;
    cam.options->video_height = CAMERA_HEIGHT;
    cam.options->framerate = CAMERA_FRAMERATE;
    cam.options->verbose = true;
    cam.options->list_cameras = true;

    cam.startVideo();

    auto start_cpu_time = std::chrono::high_resolution_clock::now();

    while (true) {
        if (!cam.getVideoFrame(image, 1000)) {
            std::cout << "Timeout error" << std::endl;
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

            // std::cout << "center x: " << center.x << " center y: " << center.y << std::endl;

            // move to certain angle

            // --------------------- For drawing the path of the ball --------------------- //
            // for (size_t i = 1; i < ball_position.size(); ++i) {
            //     if (ball_position[i - 1] == cv::Point(0, 0) || ball_position[i] == cv::Point(0, 0)) {
            //         continue;
            //     }
            //     int thickness = static_cast<int>(sqrt(32.0 / (i + 1)) * 2.5);
            //     cv::line(image, ball_position[i - 1], ball_position[i], cv::Scalar(0, 0, 255), thickness);
            // }

            // cv::imshow("Video", image);
            // cv::imshow("Mask", mask);
            char key = static_cast<char>(cv::waitKey(1));
            if (key == 'q') {
                std::cout << "yippi" << std::endl;
                break;
            }

            if ((std::chrono::high_resolution_clock::now() - start_cpu_time) > std::chrono::seconds{10}) {
                break;
            }      
        }
    }

    cam.stopVideo();
    // cv::destroyWindow("Video");
    return 0;
}