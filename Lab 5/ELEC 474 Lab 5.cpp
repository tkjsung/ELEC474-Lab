// ELEC 474 Lab 5 Code: Epipolar Lines
// Author: Tom Sung

// C++ Standard Libraries
#include <iostream>
#include <stdio.h>
#include <vector>
#include <math.h>

// OpenCV Imports
#include <opencv2/opencv.hpp>
//#include <opencv2/core/core.hpp> // OpenCV Core Functionality
//#include <opencv2/highgui/highgui.hpp> // High-Level Graphical User Interface

// Use OpenCV namespace
using namespace cv;
using namespace std;

vector<Point2f> p;

// C++ Function Declaration
void epilines(String leftImg, String rightImg);
void CallBackFunc(int event, int x, int y, int flags, void* userdata);
void singlePointLine(String leftImg, String rightImg);

int main()
{
    //cout << "Testing123" << endl;
    
//    epilines("Repair_Left.jpg","Repair_Right.jpg");
    singlePointLine("Repair_Left.jpg","Repair_Right.jpg");
//    singlePointLine("Robot_Left.jpg","Robot_Right.jpg");
//    singlePointLine("Soccer_Left.jpg", "Soccer_Right.jpg");
//    singlePointLine("Workbench_Left.jpg", "Workbench_Right.jpg");
//    epilines("Repair_LLeft.jpg","Repair_RRight.jpg");
//    epilines("Robot_Left.jpg", "Robot_Right.jpg");
//    epilines("Robot_LLeft.jpg", "Robot_RRight.jpg");
//    epilines("Soccer_Left.jpg", "Soccer_Right.jpg");
//    epilines("Soccer_LLeft.jpg", "Soccer_RRight.jpg");
//    epilines("motorcycle-im0-s.png","motorcycle-im1-s.png");
//    epilines("piano-im0-s.png","piano-im1-s.png");
//    epilines("Workbench_Left.jpg","Workbench_Right.jpg");
    
    // Terminate the program
    return 0;
}

// Adopted from: https://hasper.info/opencv-draw-epipolar-lines/
void epilines(String leftImg, String rightImg)
{
    Mat left = imread(leftImg);
    Mat right = imread(rightImg);
    //    imshow("Test",left);
    //    waitKey();
    
    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    vector<Point2f> leftPoints, rightPoints;
    Mat line1, line2;
    
    int nfeatures = 500;
    float scaleFactor = 1.2f;
    int nlevels = 8;
    int edgeThreshold = 31;
    int firstLevel = 0;
    int WTA_K = 2;
    int patchSize = 31;
    
    // Feature Detector
    Ptr<FeatureDetector> detector = ORB::create(nfeatures, scaleFactor, nlevels, edgeThreshold, firstLevel, WTA_K, ORB::HARRIS_SCORE, patchSize);
    detector->detectAndCompute(left, Mat(), keypoints1, descriptors1);
    detector->detectAndCompute(right, Mat(), keypoints2, descriptors2);
    
    // Match features
    vector<DMatch> matches;
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
    matcher->match(descriptors1, descriptors2, matches, Mat());
    
    
    sort(matches.begin(), matches.end());
    // Remove not good matches
    const int numGoodMatches = matches.size() * 0.4f;
    matches.erase(matches.begin() + numGoodMatches, matches.end());
    
    
    for (size_t i = 0; i < matches.size(); i++)
    {
        leftPoints.push_back(keypoints1[matches[i].queryIdx].pt);
        rightPoints.push_back(keypoints2[matches[i].trainIdx].pt);
    }
    
    Mat f = findFundamentalMat(leftPoints, rightPoints, FM_8POINT);
    cout << f << endl;
    
    
    
    //My Code
    
    computeCorrespondEpilines(leftPoints, 1, f, line1);
    computeCorrespondEpilines(rightPoints, 2, f, line2);
    //    cout << line1 << endl;
    
    //    Scalar color(255,255,255);
    
    //    cv::line(outImg(rect2),
    //    cv::Point(0,-epilines1[i][2]/epilines1[i][1]),
    //    cv::Point(img1.cols,-(epilines1[i][2]+epilines1[i][0]*img1.cols)/epilines1[i][1]),
    //    color);
    
    //    cout << line1 << endl;
    //    cout << line1.type() << endl;
    //    cout << line1(0,0) << endl;//<< " " << line1(0,1) << " " << line1(0,2) << endl;
    
    
    for( int i = 0; i< line1.rows; i++)
    {
        
        Scalar color(rand() % 255, rand() % 255, rand() % 255);
        
        line(right,
             Point(0,-line1.at<float>(i,2)/line1.at<float>(i,1)), Point(right.cols,-(line1.at<float>(i,2)+line1.at<float>(i,0)*right.cols)/line1.at<float>(i,1)),
             color,10);
        
        line(left,
             Point(0,-line2.at<float>(i,2)/line2.at<float>(i,1)), Point(left.cols,-(line2.at<float>(i,2)+line2.at<float>(i,0)*left.cols)/line2.at<float>(i,1)),
             color,10);
        
    }
    
    imshow("Right Image",right);
    namedWindow("Right Image",WINDOW_KEEPRATIO);
    waitKey();
    
    
    //      for( int i = 0; i< line2.rows; i++)
    //      {
    //          line(left,
    //               Point(0,-line2.at<float>(i,2)/line2.at<float>(i,1)), Point(left.cols,-(line2.at<float>(i,2)+line2.at<float>(i,0)*left.cols)/line2.at<float>(i,1)),
    //               Scalar(rand() % 255, rand() % 255, rand() % 255));
    //      }
    
    imshow("Left Image",left);
    namedWindow("Left Image",WINDOW_FREERATIO);
    waitKey();
    
    
    
    // Erin's Code
    /*
     //Select points on left image
     namedWindow("Left Image", WINDOW_AUTOSIZE);
     setMouseCallback("Left Image", CallBackFunc, NULL);
     imshow("Left Image", left);
     waitKey(0);
     
     
     //Draw epilines on right image from left image click
     vector<Vec3f> lines;
     computeCorrespondEpilines(p, 1, f, lines);
     for (vector<cv::Vec3f>::const_iterator it = lines.begin();
     it != lines.end(); ++it) {
     
     // draw the line between first and last column
     line(right,
     Point(0, (*it)[2] / (*it)[1]),
     Point(right.cols, ((*it)[2] + (*it)[0] * right.cols) / (*it)[1]),
     Scalar(255, 255, 255),10);
     }
     imshow("Right Image", right);
     waitKey(0);
     */
    
    
}

void singlePointLine(String leftImg, String rightImg)
{
    Mat lefti = imread(leftImg);
    Mat righti = imread(rightImg);
    Mat left, right;
 
    resize(lefti, left, Size(), 0.25, 0.25, INTER_NEAREST);
    resize(righti, right, Size(), 0.25, 0.25, INTER_NEAREST);
 
    vector<KeyPoint> keypoints1, keypoints2, keypointsGood;
    Mat descriptors1, descriptors2;
    vector<Point2f> leftPoints, rightPoints, points;
    Mat line1, line2, lines;
 
    int nfeatures = 500;
    float scaleFactor = 1.2f;
    int nlevels = 8;
    int edgeThreshold = 31;
    int firstLevel = 0;
    int WTA_K = 2;
    int patchSize = 31;
 
    // Feature Detector
    Ptr<FeatureDetector> detector = ORB::create(nfeatures, scaleFactor, nlevels, edgeThreshold, firstLevel, WTA_K, ORB::HARRIS_SCORE, patchSize);
    detector->detectAndCompute(left, Mat(), keypoints1, descriptors1);
    detector->detectAndCompute(right, Mat(), keypoints2, descriptors2);
 
    // Match features
    vector<DMatch> matches;
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
    matcher->match(descriptors1, descriptors2, matches, Mat());
 
    sort(matches.begin(), matches.end());
    // Remove not good matches
    const int numGoodMatches = matches.size() * 0.05f;
    matches.erase(matches.begin() + numGoodMatches, matches.end());
 
    for (size_t i = 0; i < matches.size(); i++)
    {
        leftPoints.push_back(keypoints1[matches[i].queryIdx].pt);
        rightPoints.push_back(keypoints2[matches[i].trainIdx].pt);
        keypointsGood.push_back(keypoints1[matches[i].queryIdx]);
    }
 
    Mat imMatches;
    drawMatches(left, keypoints1, right, keypoints2, matches, imMatches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    namedWindow("Matched Image", WINDOW_NORMAL);
    imshow("Matched Image", imMatches);
    waitKey();
 
    drawKeypoints(left, keypointsGood, left, Scalar::all(-1));
 
    Mat f = findFundamentalMat(leftPoints, rightPoints, FM_8POINT);
    cout << f << endl;
 
    namedWindow("Left Image", WINDOW_NORMAL);
    //resizeWindow("Left Image", 600, 400);
    setMouseCallback("Left Image", CallBackFunc, NULL);
    imshow("Left Image", left);
    waitKey(0);
 
    vector<Point2f> goodPoint;
    int flag = 0;
    for (int i = 0; i < leftPoints.size(); i++)
    {
        if ((leftPoints[i].x == p[0].x) && (leftPoints[i].y == p[0].y))
        {
            goodPoint.push_back(Point2f(p[0].x, p[0].y));
            flag = 1;
            cout << "Keypoint " << p << " clicked, " << leftPoints[i] << endl;
            break;
        }
    }
   
    if (flag == 0)
    {
        for (int xi = -10; xi < 10; xi++)
        {
            for (int yi = -10; yi < 10; yi++)
            {
                for (int i = 0; i < leftPoints.size(); i++)
                {
                    if ((leftPoints[i].x == (p[0].x + xi)) && (leftPoints[i].y == (p[0].y + yi)))
                    {
                        goodPoint.push_back(Point2f((p[0].x + xi), (p[0].y + yi)));
                        flag = 1;
                        cout << "Keypoint " << goodPoint << " found near the clicked point, " << p << endl;
                        break;
                    }
                    if (flag == 1) break;
                }
                if (flag == 1) break;
            }
            if (flag == 1) break;
        }
    }
 
    if (flag == 1)
    {
        computeCorrespondEpilines(goodPoint, 1, f, lines);
        cout << "Result probably good" << endl;
    }
    else
    {
        computeCorrespondEpilines(p, 1, f, lines);
        cout << "Result could be bad" << endl;
    }
    //    cout << line1 << endl;
    Scalar color(255, 255, 255);
 
    for (int i = 0; i < lines.rows; i++)
    {
 
        line(right,
            Point(0, -lines.at<float>(i, 2) / lines.at<float>(i, 1)),
            Point(right.cols, -(lines.at<float>(i, 2) + lines.at<float>(i, 0) * right.cols) / lines.at<float>(i, 1)),
            color, 3);
    }
    namedWindow("Right Image", WINDOW_NORMAL);
    //resizeWindow("Right Image", 600, 400);
    imshow("Right Image", right);
    waitKey();
 
    
    p.clear();
 
    cout << "----------------" << endl;
    
    destroyAllWindows();
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        p.push_back(Point2f(x, y));
        cout << p << endl;
    }
    
}
