#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/video.hpp>

#include <unistd.h>
#include "harrisdetector.h"

#define path1 "/home/ilya/Code/ComputerVisionProjects/Images/harris.png"
#define path2 "/home/ilya/Code/ComputerVisionProjects/Images/ugol.jpg"

using namespace cv::xfeatures2d;

int harris_test(cv::VideoCapture cap)
{
    cv::Mat frame, edges, cornerStrength;

    HarrisDetector harris;

    cv::namedWindow("Window", 1);

    for(;;){
        cap >> frame;

        cv::cvtColor(frame, edges, cv::COLOR_BGR2GRAY);
        std::vector<cv::Point> pts;

//        cv::GaussianBlur(edges, edges, cv::Size(3,3), 1.5, 1.5);
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


int main(int argc, char** argv )
{
    cv::VideoCapture cap(0);
    if(!cap.isOpened()){
        return -1;
    }

    cv::Mat frame, gray_fr, edges, cornerStrength;
    uint64 start;
    double fps;

    HarrisDetector harris;

    for(;;){
        start = cv::getTickCount();
        std::vector<cv::Point> keypoints1;
        std::vector<cv::KeyPoint> keypoints2;
        cap >> frame;

        cv::cvtColor(frame, gray_fr, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(gray_fr, gray_fr, cv::Size(5, 5), 1.5, 1.5);

        harris.detect(gray_fr);
        harris.getCorners(keypoints1, 0.01);

        harris.drawOnImage(frame, keypoints1, cv::Scalar(255, 0, 0));


        cv::Ptr<SURF> surf = SURF::create(2500.);
        surf->detect(gray_fr, keypoints2);

        cv::drawKeypoints(frame, keypoints2, frame, cv::Scalar(0, 255, 0), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
        cv::imshow("edges", frame);

        if(cv::waitKey(5) >= 0){
            break;
        }

        fps = cv::getTickFrequency() / (cv::getTickCount() - start);
        std::cout << "FPS : " << fps << std::endl;
    }

    return 0;
}



