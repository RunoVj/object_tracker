#ifndef FEATURETRACKER_H
#define FEATURETRACKER_H

#include "frameprocessor.h"
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/video.hpp>


class FeatureTracker : public FrameProcessor{
    // current and previous gray-level image
    cv::Mat gray;
    cv::Mat gray_prev;

    // tracked features from 0->1
    std::vector<cv::Point2f> points[2];

    // initial position of tracked points
    std::vector<cv::Point2f> initial;

    // detected features
    std::vector<cv::Point2f> features;
    int max_count;

    // maximum number of features to detect
    double qlevel;

    // quality level for feature detection
    double minDist;

    // min distance between two points
    // status of tracked features
    std::vector<uchar> status;

    // error in tracking
    std::vector<float> err;

    void process(cv::Mat &frame, cv::Mat &out);

public:
    FeatureTracker();
    bool addNewPoints();
    void detectFeaturePoints();
    // handle the currently tracked points
    void handleTrackedPoints(cv:: Mat &frame,
                             cv:: Mat &output);
    bool acceptTrackedPoint(int i);
};

#endif // FEATURETRACKER_H
