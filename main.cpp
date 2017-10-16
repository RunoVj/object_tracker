#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/video.hpp>

#include <unistd.h>
#include "harrisdetector.h"
#include "videoprocessor.h"
#include "featuretracker.h"


using namespace cv::xfeatures2d;

//class FrameProcessor{
//public:
//    //processing method
//    virtual void process(cv::Mat &input, cv::Mat &out) = 0;
//};

void canny(cv::Mat &img, cv::Mat &out)
{
    // Convert to gray
    if (img.channels() == 3)
    cv::cvtColor(img, out, CV_BGR2GRAY);
    // Compute Canny edges
    cv::Canny(out, out, 100, 200);
    // Invert the image
    cv::threshold(out, out, 128, 255, cv::THRESH_BINARY_INV);
}

void surf_detector(cv::Mat &img, cv::Mat &out)
{
    // Convert to gray
    cv::Mat gray;
    if (img.channels() == 3){
        cv::cvtColor(img, gray, CV_BGR2GRAY);
    }

    std::vector<cv::KeyPoint> keypoints;

    cv::Ptr<SURF> surf = SURF::create(2500.);

    surf->detect(gray, keypoints);
    img.copyTo(out);
    cv::drawKeypoints(out, keypoints, out, cv::Scalar(0, 255, 0), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
}

void harris(cv::Mat &img, cv::Mat &out)
{
    // Convert to gray
    cv::Mat gray;
    if (img.channels() == 3){
        cv::cvtColor(img, gray, CV_BGR2GRAY);
    }

    std::vector<cv::Point> keypoints;
    HarrisDetector harris;

    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 1.5, 1.5);
    harris.detect(gray);
    harris.getCorners(keypoints, 0.05);

    img.copyTo(out);
    harris.drawOnImage(out, keypoints, cv::Scalar(255, 0, 0));
}

void surf_and_harris(cv::Mat &img, cv::Mat &out)
{
    surf_detector(img, out);
    harris(out, out);
}

int main(int argc, char** argv )
{
    // Create video procesor instance
    VideoProcessor processor;
    // Create feature tracker instance
    FeatureTracker tracker;
    // Open video file
    processor.setInput("WEB_CAMERA");
    // set frame processor
    processor.setFrameProcessor(&tracker);
    // Declare a window to display the video
    processor.displayOutput("Tracked Features");
    // Play the video at the original frame rate
    processor.setDelay(1000./processor.getFrameRate());
    // Start the process
    processor.run();




//    // Create instance
//    VideoProcessor processor;
//    // Open video file
//    processor.setInput("WEB_CAMERA");

//    // Declare a window to display the video
//    processor.displayInput("Current Frame");
//    processor.displayOutput("Output Frame");

//    // Play the video at the original frame rate
//    processor.setDelay(1000. / processor.getFrameRate());

//    // Set the frame processor callback function
//    processor.setFrameProcessor(surf_and_harris);

//    // Start the proces
//    processor.run();

    return 0;
}



