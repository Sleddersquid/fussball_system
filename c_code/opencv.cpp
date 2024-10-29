#include <opencv2/opencv.hpp>
#include <lccv.hpp>
// #include "lccv.hpp"

int main()
{
    std::cout<<"Sample program for LCCV video capture"<<std::endl;
    std::cout<<"Press ESC to stop."<<std::endl;
    cv::Mat image;
    lccv::PiCamera cam;

    cam.options->video_width=640;
    cam.options->video_height=480;
    cam.options->framerate=100;
    cam.options->verbose=true;

    cv::namedWindow("Video",cv::WINDOW_NORMAL);
    cam.startVideo();
    int ch=0;

    while(ch!=27){
        if(!cam.getVideoFrame(image,1000)){
            std::cout<<"Timeout error"<<std::endl;
        }
        else{
            cv::imshow("Video",image);
            ch=cv::waitKey(10);
        }
    }
    cam.stopVideo();
    cv::destroyWindow("Video");
}