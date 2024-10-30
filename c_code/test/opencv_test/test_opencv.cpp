#include <iostream>

#include <opencv2/opencv.hpp>
#include <lccv.hpp>


#define CAMERA_HEIGHT 720       // Can be SD: 480, HD: 720, FHD: 1080, QHD: 1440
#define CAMERA_WIDTH 1280       // Can be SD: 640, HD: 1280, FHD: 1920, QHD: 2560
#define CAMERA_FRAMERATE 120

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

    cv::namedWindow("Video", cv::WINDOW_NORMAL);
    cam.startVideo();
    int ch = 0;

    auto start_cpu_time = std::chrono::high_resolution_clock::now();

    while (ch != 27) {
        if (!cam.getVideoFrame(image, 1000)) {
            std::cout << "Timeout error" << std::endl;
        }
        else {
            cv::imshow("Video", image);
        }
    }

    cv::destroyAllWindows();
    std::cout << "End of file" << std::endl;
    return 0;
}
