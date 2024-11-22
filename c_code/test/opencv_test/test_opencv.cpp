#include <iostream>

#include <opencv2/opencv.hpp>
#include <lccv.hpp>

#include <chrono>


#define CAMERA_HEIGHT 720       // Can be SD: 480, HD: 720, FHD: 1080, QHD: 1440
#define CAMERA_WIDTH 1280       // Can be SD: 640, HD: 1280, FHD: 1920, QHD: 2560
#define CAMERA_FRAMERATE 120

cv::Mat image;
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

    cv::namedWindow("Video", cv::WINDOW_AUTOSIZE);
    cam.startVideo();

    auto start_cpu_time = std::chrono::high_resolution_clock::now();

    while (true) {
        if (!cam.getVideoFrame(image, 100)) {
            std::cout << "Timeout error" << std::endl;
        }
        else {
            int thickness = 20;
            // Point 1: (80, 800) and (1155, 707), 2: (80, 752) and (1155, 707), 3: (85, 529) and (1152, 478)
            cv::line(image, cv::Point(85 ,529), cv::Point(1152, 478), cv::Scalar(0, 0, 255), thickness); // Full row 1
            cv::line(image, cv::Point(85, 760), cv::Point(1167, 709), cv::Scalar(0, 0, 255), thickness); // Full row 0

            cv::imshow("Video", image);

            if (cv::waitKey(1) == 'q') {
                break;
            }
        }
    }
    
    cam.stopVideo();
    cv::destroyAllWindows();
    std::cout << "End of file" << std::endl;
    return 0;
}
