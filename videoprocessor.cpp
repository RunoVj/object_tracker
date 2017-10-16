#include "videoprocessor.h"

void VideoProcessor::setFrameProcessor(FrameProcessor *frameProcessorPtr)
{
    //invalidate callback function
    process = 0;

    //this is the frame processor instance
    //that will be called
    frameProcessor = frameProcessorPtr;
    callProcess();
}

VideoProcessor::VideoProcessor()
    : callIt(true), delay(0), fnumber(0), stop(false), frameToStop(-1)
{
}

void VideoProcessor::setFrameProcessor(
        void (*frameProcessingCallback) (cv::Mat&, cv::Mat&))
{
    // invalidate frame processor class instance
    frameProcessor = 0;

    // this is the frame processor function that
    // will be called
    process = frameProcessingCallback;
    callProcess();
}

bool VideoProcessor::setInput(std::string filename)
{
    fnumber = 0;

    // In case a resource was already
    // associated with the VideoCapture instance
    capture.release();

    if (filename == "WEB_CAMERA"){
        cv::VideoCapture cap(0);
        capture = cap;
        return capture.isOpened();
    }
    else{
        // Open the video file
        return capture.open(filename);
    }
}

void VideoProcessor::displayInput(std::string wn)
{
    windowNameInput = wn;
    cv::namedWindow(windowNameInput);
}

void VideoProcessor::displayOutput(std::string wn)
{
    windowNameOutput= wn;
    cv::namedWindow(windowNameOutput);
}

void VideoProcessor::dontDisplay()
{
   cv::destroyWindow(windowNameInput);
   cv::destroyWindow(windowNameOutput);
   windowNameInput.clear();
   windowNameOutput.clear();
}

void VideoProcessor::run()
{
   // current frame
   cv::Mat frame;
   // output frame
   cv::Mat output;

   // if no capture device has been set
   if (!isOpened()){
       return;
   }

   stop = false;
   while (!isStopped()) {
       // read next frame if any
       if (!readNextFrame(frame)){
           break;
       }

       // display input frame
       if (windowNameInput.length()!=0){
           cv::imshow(windowNameInput, frame);
       }

       // calling the process function or method
       if (callIt) {
           // process the frame
           // if call back function
           if (process){
               process(frame, output);
           }
           // if class interface instance
           else if(frameProcessor){
               frameProcessor->process(frame, output);
               ++fnumber;
           }
       }
       else {
           output = frame;
       }

       // display output frame
       if (windowNameOutput.length() != 0){
           cv::imshow(windowNameOutput, output);
       }


       // introduce a delay
       if (delay >= 0 && cv::waitKey(delay) >= 0){
           stopIt();
       }

       // check if we should stop
       if (frameToStop >= 0 && getFrameNumber() == frameToStop){
           stopIt();
       }
   }
}

// Stop the processing
void VideoProcessor::stopIt()
{
    stop = true;
}

// Is the process stopped?
bool VideoProcessor::isStopped()
{
    return stop;
}

// Is a capture device opened?
bool VideoProcessor::isOpened()
{
    capture.isOpened();
}

// set a delay between each frame
// 0 means wait at each frame
// negative means no delay
void VideoProcessor::setDelay(int d)
{
    delay= d;
}

// to get the next frame
// could be: video file or camera
bool VideoProcessor::readNextFrame(cv::Mat& frame)
{
    return capture.read(frame);
}

// process callback to be called
void VideoProcessor::callProcess()
{
    callIt = true;
}

// do not call process callback
void VideoProcessor::dontCallProcess() {
    callIt = false;
}

void VideoProcessor::stopAtFrameNo(long frame)
{
    frameToStop = frame;
}

// return the frame number of the next frame
long VideoProcessor::getFrameNumber()
{
    // get info of from the capture device
    long fnumber = static_cast<long>(capture.get(CV_CAP_PROP_POS_FRAMES));
    return fnumber;
}

double VideoProcessor::getFrameRate()
{
    return capture.get(CV_CAP_PROP_FPS);
}

