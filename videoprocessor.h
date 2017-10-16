#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "frameprocessor.h"

class VideoProcessor
{
private:
    // the OpenCV video capture object
    cv::VideoCapture capture;

    // the callback function to be called
    // for the processing of each frame
    void (*process)(cv::Mat&, cv::Mat&);

    FrameProcessor *frameProcessor;

    // a bool to determine if the
    // process callback will be called
    bool callIt;

    // Input display window name
    std::string windowNameInput;

    // Output display window name
    std::string windowNameOutput;

    // delay between each frame processing
    int delay;

    // number of processed frames
    long fnumber;

    // stop at this frame number
    long frameToStop;

    // to stop the processing
    bool stop;

    // number of digits in output image filename
    int digits;

public:
    VideoProcessor();
    // set the callback function that
    // will be called for each frame
    void setFrameProcessor(void (*frameProcessingCallback) (cv::Mat&, cv::Mat&));

    //set the instance of the class that
    //implements the FrameProcessor interaface
    void setFrameProcessor(FrameProcessor *frameProcessorPtr);

    // set the name of the video file
    bool setInput(std::string filename);

    // to display the processed frames
    void displayInput(std::string wn);

    // to display the processed frames
    void displayOutput(std::string wn);

    // do not display the processed frames
    void dontDisplay();

    // to grab (and process) the frames of the sequence
    void run();

    // Stop the processing
    void stopIt();

    // Is the process stopped?
    bool isStopped();

    // Is a capture device opened?
    bool isOpened();

    // set a delay between each frame
    // 0 means wait at each frame
    // negative means no delay
    void setDelay(int d);

    // to get the next frame
    // could be: video file or camera
    bool readNextFrame(cv::Mat& frame);

    // process callback to be called
    void callProcess();

    // do not call process callback
    void dontCallProcess();

    void stopAtFrameNo(long frame);

    // return the frame number of the next frame
    long getFrameNumber();

    double getFrameRate();

};

#endif // VIDEOPROCESSOR_H
