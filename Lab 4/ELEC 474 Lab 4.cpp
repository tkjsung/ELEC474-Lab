// ELEC 474 Lab 4 Code: Feature Matching-based Object Detection and Pose Estimation
// Author: Tom Sung

// C++ Standard Libraries
#include <iostream>
#include <stdio.h>
#include <vector>
#include <math.h>
//#include <corecrt_math_defines.h>

// OpenCV Imports
#include <opencv2/opencv.hpp>
//#include <opencv2/core/core.hpp> // OpenCV Core Functionality
//#include <opencv2/highgui/highgui.hpp> // High-Level Graphical User Interface

// Use OpenCV namespace
using namespace cv;
using namespace std;

// C++ Function Declaration
void detectImage(String modelName, String targetName);
void detectImage2(String modelName, String targetName, int id);
void ransac(vector<DMatch> matches, vector<KeyPoint> keypoints1, vector<KeyPoint> keypoints2,
            Mat modelImg, Mat targetImg, int id, String modelName, String targetName);
Mat getSimilarityMatrix(vector<Point2f> ptsA, vector<Point2f> ptsB, int nPts);
Mat transformCalc(Mat &similar, vector<Point2f> &pt, int k);

int main()
{

    detectImage("box.png", "box_in_scene.png");
    detectImage("stapleRemover.jpg","clutteredDesk.jpg");
    detectImage2("66_6_PS_Dataset_Mod.png","66_5_PS_Dataset.png",2);
    
    // Terminate the program
    return 0;
}

// This section is adopted from: https://www.learnopencv.com/image-alignment-feature-based-using-opencv-c-python/
void detectImage(String modelName, String targetName)
{
    Mat modelImg = imread(modelName, IMREAD_GRAYSCALE);
    Mat targetImg = imread(targetName, IMREAD_GRAYSCALE);
    Mat imgReg, h;
    
    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    
    int nfeatures = 500;
    float scaleFactor = 1.2f;
    int nlevels = 8;
    int edgeThreshold = 31;
    int firstLevel = 0;
    int WTA_K = 2;
    int patchSize = 31;
    
    // Feature Detector
    Ptr<FeatureDetector> detector = ORB::create(nfeatures, scaleFactor, nlevels, edgeThreshold, firstLevel, WTA_K, ORB::HARRIS_SCORE, patchSize);
    detector->detectAndCompute(modelImg, Mat(), keypoints1, descriptors1);
    detector->detectAndCompute(targetImg, Mat(), keypoints2, descriptors2);
    
    // Match features
    vector<DMatch> matches;
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
    matcher->match(descriptors1, descriptors2, matches, Mat());
   
    // Oct.20 Code for match optimization
    // Sort matches by score to get top matches here...
    sort(matches.begin(), matches.end());
    // Remove not good matches
    const int numGoodMatches = matches.size() * 0.15f;
    matches.erase(matches.begin() + numGoodMatches, matches.end());
    
    
    // For painting
    ransac(matches, keypoints1, keypoints2, modelImg, targetImg, 3, modelName, targetName);
    
    
    // Draw matches
    /*
    Mat imgMatch;
    drawMatches(modelImg, keypoints1, targetImg, keypoints2, matches, imgMatch);
    imshow("Matched Image", imgMatch);
    waitKey();
    */
    
    // The following is a check for in-lab activity
    
    // Extract location of good matches
    vector<Point2f> points1, points2;
    
    for (size_t i = 0; i < matches.size(); i++)
    {
        points1.push_back(keypoints1[matches[i].queryIdx].pt);
        points2.push_back(keypoints2[matches[i].trainIdx].pt);
    }
    
    // Find homography
    h = findHomography(points1, points2, RANSAC);
    
    // Use homography to warp image
    warpPerspective(modelImg, imgReg, h, targetImg.size());
    
    imshow("Homography", imgReg);
    waitKey();
    
}

void detectImage2(String modelName, String targetName, int id)
{
    Mat modelImg = imread(modelName, IMREAD_GRAYSCALE);
    Mat targetImg = imread(targetName, IMREAD_GRAYSCALE);
    Mat imgReg, h;
    
    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    
    int nfeatures = 500;
    float scaleFactor = 1.2f;
    int nlevels = 8;
    int edgeThreshold = 31;
    int firstLevel = 0;
    int WTA_K = 2;
    int patchSize = 31;
    
    // Feature Detector
    Ptr<FeatureDetector> detector = ORB::create(nfeatures, scaleFactor, nlevels, edgeThreshold, firstLevel, WTA_K, ORB::HARRIS_SCORE, patchSize);
    detector->detectAndCompute(modelImg, Mat(), keypoints1, descriptors1);
    detector->detectAndCompute(targetImg, Mat(), keypoints2, descriptors2);
    
    // Match features
    vector<DMatch> matches;
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
    matcher->match(descriptors1, descriptors2, matches, Mat());

    
    // Oct.20 Code for match optimization
    // Sort matches by score to get top matches here...
    sort(matches.begin(), matches.end());
    // Remove not good matches
    const int numGoodMatches = matches.size() * 0.15f;
    matches.erase(matches.begin() + numGoodMatches, matches.end());
    
    
    
    // For painting
    ransac(matches, keypoints1, keypoints2, modelImg, targetImg, id, modelName, targetName);
    
    
    // Draw matches
    /*
    Mat imgMatch;
    drawMatches(modelImg, keypoints1, targetImg, keypoints2, matches, imgMatch);
    imshow("Matched Image", imgMatch);
    waitKey();
    */
    
    // The following is a check for in-lab activity
    
    // Extract location of good matches
    vector<Point2f> points1, points2;
    
    for (size_t i = 0; i < matches.size(); i++)
    {
        points1.push_back(keypoints1[matches[i].queryIdx].pt);
        points2.push_back(keypoints2[matches[i].trainIdx].pt);
    }
    
    // Find homography
    h = findHomography(points1, points2, RANSAC);
    
    // Use homography to warp image
    warpPerspective(modelImg, imgReg, h, targetImg.size());
    
    imshow("Homography", imgReg);
    waitKey();
    
}

void ransac(vector<DMatch> matches, vector<KeyPoint> keypoints1, vector<KeyPoint> keypoints2,
            Mat modelImg, Mat targetImg, int id, String modelName, String targetName)
{
    // Initializing variables
    int k = 1000; // number of iterations
    int m = 2; // min number of required points to compute needed transformation
    int t = 60; // distance threshold used to identify a correspondence as an inlier
    float p; // probability of inliers with respect to the whole set
    float p_th = 0.75; // min probability of inliers to state that a transforamtion is good
    Mat outputImg, h;
    
    // Box values
//    if (id == 1){
//        t = 52;
//        p_th = 0.85;
//    }
    
    // Stapler values
    if (id == 3)
    {
        t = 15;
        p_th = 0.67;
    }
    
    // Painting
    if (id == 2 || id == 1)
    {
        k = 500;
        p_th = 0.8;
        t = 50;
    }
    
    // Get the point queryIdx/trainIdx points
    vector<Point2f> kpts1_matched, kpts2_matched;
    
    Mat transResult1, transResult2;
    Mat goodTrans;
    
    // Index all the keypoints into a vector, with each array slot allocated for a x,y point.
    for (int i = 0; i < (int)matches.size(); i++)
    {
        kpts1_matched.push_back(keypoints1[matches[i].queryIdx].pt);
        kpts2_matched.push_back(keypoints2[matches[i].trainIdx].pt);
        
    }

    RNG rng((uint64)-1);
    // Now, do the k iterations
    for (int i = 0; i < k ; i++)
    {
        vector<Point2f> newPoint1, newPoint2;
        vector<Point> listofPoints, targetPoints;
        Point p1, p2, p3, p4;
        // Random Number Generator Setup
        int idx1 = (int)rng.uniform(0, (int)matches.size());
        int idx2 = (int)rng.uniform(0, (int)matches.size());
        
        // Need only two points for transformation calculation
        newPoint1.push_back(keypoints1[matches[idx1].queryIdx].pt);
        newPoint1.push_back(keypoints1[matches[idx2].queryIdx].pt);
        newPoint2.push_back(keypoints2[matches[idx1].trainIdx].pt);
        newPoint2.push_back(keypoints2[matches[idx2].trainIdx].pt);
        
        // Compute the transformation using SVD (getSimilarityMatrix as given in lab)
        //Mat similar = getSimilarityMatrix(kpts1_matched, kpts2_matched, 2);
        Mat similar = getSimilarityMatrix(newPoint1, newPoint2, m);
        
        // Transforming the points...
        /* Steps
         1. Similarity matrix needs to be applied to model image (the "matches" points).
         2. Calculating distances between two "matches" points on the model image
         3. Compare that distance with the target image (same "matches" indices)
            If it's within threshold, then it is an inlier.
         4. Repeat steps 2-4 'k' times to find number of inliers
         5. Calculate probability p: p = inliers/(total # of matches)
         6. If probability is high (above probability threshold), then we know the
            SimilarityMatrix is a good transformation
        */
        for (int j = 0; j < kpts1_matched.size(); j++)
        {
            transResult1 = transformCalc(similar, kpts1_matched, j); // model image
            //transResult2 = transformCalc(similar, kpts2_matched, j); // target image

            listofPoints.push_back(Point(transResult1.at<float>(0,0),
                                         transResult1.at<float>(1,0)));
            targetPoints.push_back(Point(kpts2_matched[j].x,kpts2_matched[j].y));
        }
        
        RNG rng2((uint64)-1);
        int inliers = 0;
        
        for (int j = 0; j < listofPoints.size(); j++)
        {
            inliers = 0;
            int idx3 = j;
            int idx4 = j + 1;
//            int idx3 = (int)rng2.uniform(0, (int)listofPoints.size());
//            int idx4 = (int)rng2.uniform(0, (int)listofPoints.size());
            p1 = listofPoints[idx3];
            p2 = listofPoints[idx4];
            p3 = targetPoints[idx3];
            p4 = targetPoints[idx4];
            
            // RANSAC algorithm from Lab 3
            if (norm(p1-p2) < t && norm(p3-p4) < t) continue;
            
            float a = static_cast<float>(p2.y - p1.y);
            float b = static_cast<float>(p1.x - p2.x);
            float c = static_cast<float>(p2.x * p1.y - p1.x * p2.y);
            float a2 = static_cast<float>(p4.y - p3.y);
            float b2 = static_cast<float>(p3.x - p4.x);
            float c2 = static_cast<float>(p4.x * p3.y - p3.x * p4.y);
            //normalize them
            float scale = 1.f / sqrt(a * a + b * b);
            float scale2 = 1.f / sqrt(a2 * a2 + b2 * b2);
            a *= scale;
            b *= scale;
            c *= scale;
            a2 *= scale2;
            b2 *= scale2;
            c2 *= scale2;
            
            for (int l = 0; l < listofPoints.size(); l++)
            {
                Point point  = listofPoints[l];
                Point point2 = targetPoints[l];
                float distance = abs(a * point.x + b * point.y + c);
                float distance2 = abs(a2 * point2.x + b2 * point2.y + c);
                
                distance = abs(distance-distance2);
                
                if(distance < t) inliers++;

            }
            
            // Probability
            p = (float)inliers / matches.size();
            
//            warpPerspective(modelImg, outputImg, similar, targetImg.size());
//            imshow("RANSAC", outputImg);
//            waitKey(1);
            
            if(p > p_th)
            {
                p_th = p;
                goodTrans = similar;
                //cout << "Probability Threshold " << p_th << endl;
            }
            else
            {
                continue;
            }
        }
    }
    
    
    h = findHomography(kpts1_matched, kpts2_matched, RANSAC);
    cout << "Calculated RANSAC: \n" << goodTrans << endl;
    cout << "Homography: \n" << h << endl;


    
    // White box
    
    for (int r = 0; r < modelImg.rows; r++)
    {
        for(int c = 0; c < modelImg.cols; c++)
        {
            modelImg.at<unsigned char>(r,c) = 255;
        }
    }
    
    
    warpPerspective(modelImg, outputImg, goodTrans, targetImg.size());
    Mat blue(modelImg.rows, modelImg.cols, CV_8UC3);
    cvtColor(modelImg, modelImg, COLOR_GRAY2BGR);
    cvtColor(outputImg, outputImg, COLOR_GRAY2BGR);
    cvtColor(targetImg, targetImg, COLOR_GRAY2BGR);
    Mat newImg;
    rectangle(blue, Point(0,0), Point(modelImg.cols-1,modelImg.rows-1), Scalar(255, 0,0),20);
    warpPerspective(blue, newImg, h, targetImg.size());
    
    double alpha = 0.4;
    addWeighted(outputImg, alpha, targetImg, 1.0, 0, targetImg);
    addWeighted(targetImg, 1, newImg, 1.0, 0, targetImg);
    
    imshow("Overlay", targetImg);
    waitKey();
    
    
    // Attempt on blue box
    /*
//    Mat blueRect = Mat(modelImg.rows, modelImg.cols, CV_8UC3);
//    Mat color(blueRect.size(), CV_8UC3, Scalar(255,0,0));
    
//    Mat newTarget = imread(targetName, CV_8UC3);
//    warpPerspective(blueRect, outputImg, goodTrans, newTarget.size());
    
//    warpPerspective(modelImg, outputImg, goodTrans, targetImg.size());
//    imshow("test",outputImg);
//    waitKey();
//    Mat colour;
//    cvtColor(outputImg, colour, COLOR_GRAY2BGR);
//
//    for(int r = 0; r < outputImg.rows; r++)
//    {
//        for (int c = 0; c < outputImg.cols; c++)
//        {
//            int value = outputImg.at<unsigned char>(r,c);
//            if (value == 255)
//            {
//                colour.at<Vec3b>(r,c)[0] = 255;
//                colour.at<Vec3b>(r,c)[1] = 0;
//                colour.at<Vec3b>(r,c)[2] = 0;
//            }
//        }
//    }
//    imshow("colour",colour);
//    waitKey();

//    double alpha = 1;
//    addWeighted(outputImg, alpha, newTarget, 1.0-alpha , 0.0, newTarget);
//    imshow("Test",newTarget);
//    waitKey();
        */
    
    
    warpPerspective(modelImg, outputImg, goodTrans, targetImg.size());
    imshow("RANSAC", outputImg);
    waitKey();
    
    
    //    warpPerspective(modelImg, outputImg, h, targetImg.size());
    //    imshow("Homography", outputImg);
    //    waitKey();
    

    
    // Clearing variable elements
    kpts1_matched.clear();
    kpts2_matched.clear();
    
}


Mat getSimilarityMatrix(vector<Point2f> ptsA, vector<Point2f> ptsB, int nPts)
{
    float tx, ty, scale, theta;
    int rows = 2 * nPts, cols = 5;
    Mat A(rows, cols, CV_32FC1);
    
    for (int i = 0; i < nPts; i++)
    {
        A.at<float>(2 * i, 0) = ptsA[i].x;
        A.at<float>(2 * i, 1) = -ptsA[i].y;
        A.at<float>(2 * i, 2) = 1.0;
        A.at<float>(2 * i, 3) = 0.0;
        A.at<float>(2 * i, 4) = -ptsB[i].x;
        
        A.at<float>(2 * i + 1, 0) = ptsA[i].y;
        A.at<float>(2 * i + 1, 1) = ptsA[i].x;
        A.at<float>(2 * i + 1, 2) = 0.0;
        A.at<float>(2 * i + 1, 3) = 1.0;
        A.at<float>(2 * i + 1, 4) = -ptsB[i].y;
    }
    
    SVD svd;
    Mat X;
    svd.solveZ(A, X);
    
    float sa = X.at<float>(0) / X.at<float>(4);
    float sb = X.at<float>(1) / X.at<float>(4);
    tx = X.at<float>(2) / X.at<float>(4);
    ty = X.at<float>(3) / X.at<float>(4);
    
    scale = (float)sqrt((double)(sa * sa + sb * sb));
    theta = atan2(sb, sa) * 180.0 / M_PI;
    
    Mat M(3, 3, CV_32FC1);
    M.at<float>(0, 0) = sa;
    M.at<float>(0, 1) = -sb;
    M.at<float>(0, 2) = tx;
    M.at<float>(1, 0) = sb;
    M.at<float>(1, 1) = sa;
    M.at<float>(1, 2) = ty;
    // Concatenate last row for projection
    M.at<float>(2, 0) = 0;
    M.at<float>(2, 1) = 0;
    M.at<float>(2, 2) = 1;
    
    return(M);
}

Mat transformCalc(Mat &similar, vector<Point2f> &pt, int k)
{
    float a,b,c;
    Mat M(3,1,CV_32FC1);
    
    a = similar.at<float>(0,0) * pt[k].x;
    b = similar.at<float>(0,1) * pt[k].y;
    c = similar.at<float>(0,2);
    M.at<float>(0,0) = a + b + c;
    
    a = similar.at<float>(1,0) * pt[k].x;
    b = similar.at<float>(1,1) * pt[k].y;
    c = similar.at<float>(1,2);
    M.at<float>(1,0) = a + b + c;
    
    M.at<float>(2,0) = 0;
    //cout << final << endl;
    
    return(M);
}

