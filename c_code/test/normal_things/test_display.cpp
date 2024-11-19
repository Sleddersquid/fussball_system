#include <opencv2/opencv.hpp>
#include <lccv.hpp>
// #include "lccv.hpp"

// avg FPS at HD is 125 without computhon center, with center computing, 111.111, and at FHD is 48.478 
#define CAMERA_HEIGHT 720       // Can be SD: 480, HD: 720, FHD: 1080, QHD: 1440
#define CAMERA_WIDTH 1280       // Can be SD: 640, HD: 1280, FHD: 1920, QHD: 2560
#define CAMERA_FRAMERATE 120

int main()
{
    std::cout<<"Sample program for LCCV video capture"<<std::endl;
    std::cout<<"Press ESC to stop."<<std::endl;
    cv::Mat image;
    lccv::PiCamera cam;

    cam.options->video_width=CAMERA_WIDTH;
    cam.options->video_height=CAMERA_HEIGHT;
    cam.options->framerate=CAMERA_FRAMERATE;
    cam.options->verbose=true;

    cv::namedWindow("Video",cv::WINDOW_NORMAL);
    cam.startVideo();
    int ch=0;

    while(ch!=27){
        if(!cam.getVideoFrame(image,1000)){
            std::cout<<"Timeout error"<<std::endl;
        }
        else{
            cv::line(image, cv::Point(0,0), cv::Point(100,100), cv::Scalar(0,0,255), 2);

            cv::imshow("Video",image);
            ch=cv::waitKey(10);

        }
    }
    cam.stopVideo();
    cv::destroyWindow("Video");
}