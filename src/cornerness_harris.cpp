#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>

using namespace std;

void cornernessHarris()
{
    // load image from file
    cv::Mat img;
    img = cv::imread("../images/img1.png");
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY); // convert to grayscale

    // Detector parameters
    int blockSize = 2;     // for every pixel, a blockSize × blockSize neighborhood is considered
    int apertureSize = 3;  // aperture parameter for Sobel operator (must be odd)
    int minResponse = 100; // minimum value for a corner in the 8bit scaled response matrix
    double k = 0.04;       // Harris parameter (see equation for details)

    // Detect Harris corners and normalize output
    cv::Mat dst, dst_norm, dst_norm_scaled;
    dst = cv::Mat::zeros(img.size(), CV_32FC1);
    cv::cornerHarris(img, dst, blockSize, apertureSize, k, cv::BORDER_DEFAULT);
    cv::normalize(dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat());
    cv::convertScaleAbs(dst_norm, dst_norm_scaled);

    

    // TODO: Your task is to locate local maxima in the Harris response matrix 
    // and perform a non-maximum suppression (NMS) in a local neighborhood around 
    // each maximum. The resulting coordinates shall be stored in a list of keypoints 
    // of the type `vector<cv::KeyPoint>`.
    std::vector<cv::KeyPoint> corners;

    int maxCorners = 100; //max corners to return , if more corners are found, strongest are returned.
    double qualityLevel = 0.01; //doubtful
    double minDistance = 0.5;//euclidean distanve between the returned corners.
    bool useHarrisDetector=false;
    //cv::goodFeaturesToTrack(dst_norm_scaled, corners,maxCorners,qualityLevel,minDistance,cv::Mat(),blockSize,useHarrisDetector,k);
    //convert keypoint to point 2f
    std::vector<cv::Point2f> corner_points;
    std::vector<cv::KeyPoint>::iterator it;
    cv::goodFeaturesToTrack(dst_norm_scaled, corner_points,maxCorners,qualityLevel,minDistance,cv::Mat(),blockSize,useHarrisDetector,k);
    for(it=corners.begin();it!=corners.end();it++)
    corner_points.push_back(it->pt);


    //drawing circles around corners
    int radius = 1;
    int thickness = 1;
    int lineType=cv::LINE_8;
    int shift = 0;
    for(int i=0;i<maxCorners;i++)
    cv::circle(dst_norm_scaled,corner_points[i],1,CV_RGB(0,0,255),thickness,lineType,shift);

    // visualize results
    string windowName = "Harris Corner Detector Response Matrix";
    cv::namedWindow(windowName, 4);
    cv::imshow(windowName, dst_norm_scaled);
    cv::waitKey(0);

}

int main()
{
    cornernessHarris();
}