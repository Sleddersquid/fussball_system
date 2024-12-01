#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>

// CHATGPT
// Tried to open the camera, but failed.

int main() {
    // Open the default camera (0)
    cv::VideoCapture cap(0);

    // Check if camera opened successfully
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open the camera!" << std::endl;
        return -1;
    }

    // Release the camera and close any open windows
    cap.release();
    cv::destroyAllWindows();

    return 0;
}
