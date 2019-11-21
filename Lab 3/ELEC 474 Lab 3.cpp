// ELEC-474 OpenCV Hello World.cpp
// ELEC 474 Lab 3 Code
// Author: Tom Sung

// The following code is what I found online that contains the entirety of RANSAC.
// https://stackoverflow.com/questions/21020496/opencv-ransac-fitting-line

// C++ Standard Libraries
#include <iostream>

// OpenCV Imports
#include <opencv2/core/core.hpp> // OpenCV Core Functionality
#include <opencv2/highgui/highgui.hpp> // High-Level Graphical User Interface
#include <opencv2/opencv.hpp>
#include <list>
#include <math.h>

// Using OpneCV namespace
using namespace cv;
using namespace std;


// C++ Function Declaration
void lab(String file);
Mat loadImage(String img);
Mat cannyImg(Mat img);
void ransac(Mat img, Mat colourImg);
void circles(String img, int select);

// Global Variable Declaration
int thresh1 = 150;
int thresh2 = 400;



// Main Function
int main()
{

	lab("road_1.jpeg");
	lab("road_2.jpeg");
	thresh1 = 300;
	thresh2 = 400;
	lab("road_3.jpeg");
	lab("road_4.jpeg");
	
	//circles("Planets.jpg", 1);
	//circles("Planets.jpg", 2);

	// Terminate the program
	return 0;
}

void lab(String file)
{
	Mat img = loadImage(file);
	Mat colourImg = imread(file);
	Mat cannyEdgeMap = cannyImg(img);
	ransac(cannyEdgeMap, colourImg);
}

Mat loadImage(String img)
{
    Mat colourImg = imread(img);
    Mat grayscaleImg;
    cvtColor(colourImg,grayscaleImg,COLOR_RGB2GRAY);
    /*namedWindow("Grayscale",WINDOW_AUTOSIZE);
    imshow("Grayscale",grayscaleImg);
    waitKey();*/
    return grayscaleImg;
}

Mat cannyImg(Mat img)
{
    Mat canny_output;
    Canny(img, canny_output, thresh1, thresh2);
    namedWindow("Canny", WINDOW_AUTOSIZE);
    imshow("Canny", canny_output);
    waitKey();
    return canny_output;
}

void DrawLine(Mat img, Point p1, Point p2)
{
    int thickness = 2;
    int lineType = 8;
    line(img,
        p1,
        p2,
		255,
        //Scalar(255, 255, 255),
        thickness,
        lineType);
}

//struct Line {
//    Point p1;
//    Point p2;
//};

void ransac(Mat img, Mat colourImg)
{
    int n = 2;
    int k = 5000;
    float t = 0.65;
    int d = 120;



	t = 1;
	d = 110;
	k = 10000;

    Mat ransacLines = Mat::zeros(img.size(), CV_8UC3);

	//vector<vector<Point> > contours;
	//vector<Vec4i> hierarchy;

    //Create list of points
    vector<Point> listOfPoints;
    //vector<Line> lineCandidates;
    RNG rng((uint64)-1);

    for (int r = 0; r < img.rows; r++)
    {
        for (int c = 0; c < img.cols; c++)
        {
            if (img.at<unsigned char>(r, c) == 255)
            {
                listOfPoints.push_back(Point(c, r));
            }
        }
    }
    
    for (int i = 0; i < k; i++)
    {
        int inliers = 0;

        //Choose n points randomly
        int idx1 = (int)rng.uniform(0, (int)listOfPoints.size());
        int idx2 = (int)rng.uniform(0, (int)listOfPoints.size());
        Point p1 = listOfPoints[idx1];
        Point p2 = listOfPoints[idx2];

        //Fit line to these points
        if (norm(p1-p2) < t)
        {
            continue;
        }

        float a = static_cast<float>(p2.y - p1.y);
        float b = static_cast<float>(p1.x - p2.x);
        float c = static_cast<float>(p2.x * p1.y - p1.x * p2.y);
        //normalize them
        float scale = 1.f / sqrt(a * a + b * b);
        a *= scale;
        b *= scale;
        c *= scale;

        //For all points
        for (int j = 0; j < listOfPoints.size(); j++)
        {
            Point point = listOfPoints[j];
            float distance = abs(a * point.x + b * point.y + c);
            if (distance < t)
            {
                inliers++;
            }
        }

        //If d or more points close to the line, is a good fit - is d minimum to be a good fit or is d the last best?
        if (inliers >= d)
        {
            //If good fit, refit line with all points and add to list of good lines so next part can be removed from loop and all lines can be drawn at once
			
			/*
			findContours(img, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
			int idx = 0;
			for (; idx >= 0; idx = hierarchy[idx][0])
			{
				Scalar color(0, 0, 255);
				drawContours(ransacLines, contours, idx, color, FILLED, 8, hierarchy);
			}
			*/

			DrawLine(colourImg, p2, p1);
        }
    }

    namedWindow("RANSAC", WINDOW_AUTOSIZE);
    //imshow("RANSAC", ransacLines);
	imshow("RANSAC", colourImg);
    waitKey();
}

struct Circle {
	Point p1;
	Point p2;
	Point p3;
};

void circles(String img, int select)
{
	Mat image;
	image = imread(img, cv::IMREAD_UNCHANGED);

	Mat imageGray = image.clone();
	cvtColor(image, imageGray, COLOR_BGR2GRAY);
	Mat imageCanny = imageGray.clone();

	GaussianBlur(imageGray, imageGray, Size(3, 3), 0);
	//GaussianBlur(imageGray, imageGray, Size(5, 5), 0);

	Canny(imageGray, imageCanny, 175, 350);
	//Mat ransacLines = Mat::zeros(imageGray.size(), CV_8UC3);
	//Mat imageContour = Mat::zeros(imageGray.size(), CV_8UC1);

	vector<Point> listOfPoints;
	vector<Circle> circleCandidates;
	//vector<Vec4i>hierarchy;
	//vector<vector<Point> > contours;
	RNG rng((uint64)-1);

	//findContours(imageCanny, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	//drawContours(imageContour, contours, -1, Scalar(255,255,255), 6);


	// Original Circle RANSAC
	int n = 3;
	int times = 1000; // this is k
	float t = 2;
	int numPoints = 500; // this is d
	
	// Improved version
	if (select == 2)
	{
		n = 3;
		times = 10000; // this is k
		t = 0.3;
		numPoints = 50; // this is d
	}

	for (int r = 0; r < imageCanny.rows; r++)
	{
		for (int c = 0; c < imageCanny.cols; c++)
		{
			if (imageCanny.at<unsigned char>(r, c) == 255)
			{
				listOfPoints.push_back(Point(c, r));
			}
		}
	}

	for (int i = 0; i < times; i++)
	{
		int inliers = 0;

		//Choose n points randomly
		int idx1 = (int)rng.uniform(0, (int)listOfPoints.size());
		int idx2 = (int)rng.uniform(0, (int)listOfPoints.size());
		int idx3 = (int)rng.uniform(0, (int)listOfPoints.size());
		if (idx1 == idx2) continue;
		if (idx1 == idx3) continue;
		if (idx3 == idx2) continue;
		Point p1 = listOfPoints[idx1];
		Point p2 = listOfPoints[idx2];
		Point p3 = listOfPoints[idx3];

		Point2f center;
		float radius;

		float x1 = p1.x;
		float x2 = p2.x;
		float x3 = p3.x;

		float y1 = p1.y;
		float y2 = p2.y;
		float y3 = p3.y;

		center.x = (x1 * x1 + y1 * y1) * (y2 - y3) + (x2 * x2 + y2 * y2) * (y3 - y1) + (x3 * x3 + y3 * y3) * (y1 - y2);
		center.x /= (2 * (x1 * (y2 - y3) - y1 * (x2 - x3) + x2 * y3 - x3 * y2));

		center.y = (x1 * x1 + y1 * y1) * (x3 - x2) + (x2 * x2 + y2 * y2) * (x1 - x3) + (x3 * x3 + y3 * y3) * (x2 - x1);
		center.y /= (2 * (x1 * (y2 - y3) - y1 * (x2 - x3) + x2 * y3 - x3 * y2));

		radius = sqrt((center.x - x1) * (center.x - x1) + (center.y - y1) * (center.y - y1));

		//For all points
		for (int j = 0; j < listOfPoints.size(); j++)
		{
			Point point = listOfPoints[j];
			float thing = pow((point.x - center.x), 2) + pow((point.y - center.y), 2);
			float distance = abs(sqrt(thing));
			if ((distance < (radius + t)) && (distance > (radius - t)))
			{
				inliers++;
			}
		}

		//If d or more points close to the line, is a good fit - is d minimum to be a good fit or is d the last best?
		if ((inliers >= numPoints))
		{
			//If good fit, refit line with all points and add to list of good lines so next part can be removed from loop and all lines can be drawn at once
			int thickness = 1;
			int lineType = 8;
			//Scalar(255, 255, 255);
			int blue = 255;
			circle(image, center, radius, blue, thickness, lineType);
		}
	}

	namedWindow("RANSAC", WINDOW_AUTOSIZE);
	imshow("RANSAC", image);
	//namedWindow("ORIGINAL", WINDOW_AUTOSIZE);
	//imshow("ORIGINAL", imageGray);
	namedWindow("CANNY", WINDOW_AUTOSIZE);
	imshow("CANNY", imageCanny);

	//namedWindow("CONTOURS", WINDOW_AUTOSIZE);
	//imshow("CONTOURS", imageContour);
	waitKey();
}
