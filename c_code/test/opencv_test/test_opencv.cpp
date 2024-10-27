#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Open the default camera (0)
    cv::VideoCapture cap(0);

    // Check if camera opened successfully
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open the camera!" << std::endl;
        return -1;
    }

    // Set the resolution to 480p (640x480)
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    // Create a window to display the video
    cv::namedWindow("Camera Preview", cv::WINDOW_AUTOSIZE);

    // Capture and display video frames in a loop
    while (true) {
        cv::Mat frame;
        cap >> frame;  // Capture frame from the camera

        // If the frame is empty, break the loop
        if (frame.empty()) {
            std::cerr << "Error: Empty frame!" << std::endl;
            break;
        }

        // Show the frame in the window
        cv::imshow("Camera Preview", frame);

        // Wait for 1 millisecond; if 'q' is pressed, exit the loop
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    // Release the camera and close any open windows
    cap.release();
    cv::destroyAllWindows();

    return 0;
}
