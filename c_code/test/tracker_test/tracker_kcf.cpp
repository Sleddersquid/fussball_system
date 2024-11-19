#include <opencv2/opencv.hpp>
#include <lccv.hpp>


#define CAMERA_HEIGHT 720       // Can be SD: 480, HD: 720, FHD: 1080, QHD: 1440
#define CAMERA_WIDTH 1280       // Can be SD: 640, HD: 1280, FHD: 1920, QHD: 2560
#define CAMERA_FRAMERATE 1000 // heheh    // If fps higher than what the thread can handle, it will just run lower fps.


cv:Rect roi;
cv::Mat image

Ptr<Tracker> tracker = TrackerKCF::create();




int main(int argc, char** argv) {
    lccv::PiCamera cam;

    cam.options->video_width = CAMERA_WIDTH;
    cam.options->video_height = CAMERA_HEIGHT;
    cam.options->framerate = CAMERA_FRAMERATE;
    cam.options->verbose = true;


    while(true) {
        if(!cam.getVideoFrame(image, 1000)) {  // Define which mat to load the image into, and the timeout in ms
            std::cout << "Timeout error" << std::endl;
        }
        else {

        }







    }




    return 0;
}





