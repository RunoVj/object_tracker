#include "featuretracker.h"

FeatureTracker::FeatureTracker()
    : max_count(500), qlevel(0.01), minDist(10.), cluster_count(1), cluster_treshold(5000)
{
}


void FeatureTracker::k_means_tracker(cv::Mat &frame, std::vector<cv::Point2f> &points)
{
    cv::Mat labels, centers;
    std::vector<int> lab;

    cluster_count = cv::partition<cv::Point2f>(points, lab, *(this));
    std::cout << "K = " << cluster_count << std::endl;

    int clusterCount = MIN(cluster_count, points.size());

    if (clusterCount != 0 && points.size() > clusterCount){
        cv::kmeans(points, clusterCount, labels,
            cv::TermCriteria( cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 10, 1.0),
               3, cv::KMEANS_RANDOM_CENTERS, centers);


        for(int i = 0; i < clusterCount; ++i){
            cv::circle( frame, cv::Point2f(centers.row(i)), 5, cv::Scalar(0, 0, 170), cv::FILLED, cv::LINE_AA );
            cv::circle( frame, cv::Point2f(centers.row(i)), 100, cv::Scalar(255, 255, 255), 1, cv::LINE_AA );
        }
    }

    cv::imshow("detected", frame);
}

void FeatureTracker::process(cv::Mat &frame, cv::Mat &out)
{
    // convert to gray-level image
    cv::cvtColor(frame, gray, CV_BGR2GRAY);
    frame.copyTo(out);

    // if new feature points must be added
    if(addNewPoints()){
        // detect feature points
        detectFeaturePoints();
        // add the detected features to
        // the currently tracked features
        points[0].insert(points[0].end(),
                         features.begin(),features.end());

        initial.insert(initial.end(),
                       features.begin(),features.end());
    }

    // for first image of the sequence
    if(gray_prev.empty()){
        gray.copyTo(gray_prev);
    }

    // track features
    cv::calcOpticalFlowPyrLK(
                gray_prev,
                gray, // 2 consecutive images
                points[0], // input point positions in first image
                points[1], // output point positions in the 2nd image
                status,// tracking success
                err);



    // loop over the tracked points to reject some
    int k = 0;
    for(int i = 0; i < points[1].size(); ++i) {
        // do we keep this point?
        if (acceptTrackedPoint(i)) {
            // keep this point in vector
            initial[k] = initial[i];
            points[1][k++] = points[1][i];
        }
    }


    // eliminate unsuccesful points
    points[1].resize(k);
    initial.resize(k);


    k_means_tracker(frame, points[1]);


    // handle the accepted tracked points
    handleTrackedPoints(frame, out);

    // current points and image become previous ones
    std::swap(points[1], points[0]);
    cv::swap(gray_prev, gray);
}

bool FeatureTracker::operator()(const cv::Point2f &a, const cv::Point2f &b)
{
    return ((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y)) < cluster_treshold;
}

// determine if new points should be added
bool FeatureTracker::addNewPoints() {
    // if too few points
    return points[0].size()<=10;
}

// determine which tracked point should be accepted
bool FeatureTracker::acceptTrackedPoint(int i) {
    return status[i] &&
            // if point has moved
            (abs(points[0][i].x - points[1][i].x) +
            (abs(points[0][i].y - points[1][i].y)) > 1);
}

void FeatureTracker::detectFeaturePoints() {
    // detect the features
    cv::goodFeaturesToTrack(gray, // the image
                            features, // the output detected features
                            max_count, // the maximum number of features
                            qlevel, // quality level
                            minDist); // min distance between two features

}

// handle the currently tracked points
void FeatureTracker::handleTrackedPoints(cv:: Mat &frame,
                         cv:: Mat &output)
{
    // for all tracked points
    for(int i = 0; i < points[1].size(); ++i) {
        // draw line and circle
        cv::line(output, initial[i], // initial position
                 points[1][i], // new position
                cv::Scalar(255, 255, 255));
                cv::circle(output, points[1][i], 3,
                cv::Scalar(255, 255, 255),-1);
    }
}
