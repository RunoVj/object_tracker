#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "harrisdetector.h"

#define path1 "/home/ilya/Code/ComputerVisionProjects/Images/harris.png"
#define path2 "/home/ilya/Code/ComputerVisionProjects/Images/ugol.jpg"


int main(int argc, char** argv )
{
    cv::VideoCapture cap(0);
    if(!cap.isOpened()){
        return -1;
    }
    cv::Mat frame, edges, cornerStrength;

    HarrisDetector harris;

    cv::namedWindow("Window", 1);

    for(;;){
        cap >> frame;

        cv::cvtColor(frame, edges, cv::COLOR_BGR2GRAY);
        std::vector<cv::Point> pts;

        cv::GaussianBlur(edges, edges, cv::Size(7,7), 1.5, 1.5);
//        cv::Canny(edges, edges, 0, 30, 3);

        harris.detect(edges);
        harris.getCorners(pts, 0.01);
        harris.drawOnImage(frame, pts, cv::Scalar(255, 0, 0));

        cv::imshow("edges", frame);


        if(cv::waitKey(5) >= 0){
            break;
        }
    }

    return 0;
}

