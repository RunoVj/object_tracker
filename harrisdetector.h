#ifndef HARRISDETECTOR_H
#define HARRISDETECTOR_H

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>


class HarrisDetector {
private:
    cv::Mat cornerStrength; // 32-bit float image of corner strength
    cv::Mat cornerTh; // 32-bit float image of thresholded corners

    cv::Mat localMax; // image of local maxima (internal)

    int neighbourhood; // size of neighborhood for derivatives smoothing
    int aperture; // aperture for gradient computation
    double k; // Harris parameter

    double maxStrength; // maximum strength for threshold computation
    double threshold; // calculated threshold (internal)
    int nonMaxSize; // size of neighborhood for non-max suppression
    cv::Mat kernel; // kernel for non-max suppression

public:
    HarrisDetector();

    void setLocalMaxWindowSize(int size);

    // Compute Harris corners
    void detect(const cv::Mat& image);
    cv::Mat getCornerMap(double qualityLevel);

    // Get the feature points from the computed Harris values
    void getCorners(std::vector<cv::Point> &points, double qualityLevel);
    // Get the feature points from the computed corner map
    void getCorners(std::vector<cv::Point> &points, const cv::Mat &cornerMap);
    void drawOnImage(cv::Mat &image, const std::vector<cv::Point> &points,
                     cv::Scalar color=cv::Scalar(255,255,255),
                     int radius=3, int thickness=2);
};


#endif // HARRISDETECTOR_H
