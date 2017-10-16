#ifndef FRAMEPROCESSOR_H
#define FRAMEPROCESSOR_H

#include <opencv2/core/core.hpp>

class FrameProcessor{
public:
    //processing method
    virtual void process(cv::Mat &frame, cv::Mat &out) = 0;
};

#endif // FRAMEPROCESSOR_H
