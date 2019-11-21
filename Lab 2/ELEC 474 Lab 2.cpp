// ELEC-474 OpenCV Hello World.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ELEC 474 Lab 2 Code
// Author: Tom Sung

// C++ Standard Libraries
#include <iostream>

// OpenCV Imports
#include <opencv2/core/core.hpp> // OpenCV Core Functionality
#include <opencv2/highgui/highgui.hpp> // High-Level Graphical User Interface 
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

// [Optional] Use OpenCV namespace
// NOTE! If note included, all OpenCV functions will need to be prepended with "cv::"
// EX: "Mat image;" -> "cv:Mat image;"
using namespace cv;
using namespace std;

// C++ Function Declaration
void importVideo(int &num, int &height, int &width);
void mean(Mat& frame,Mat &gray, Mat& newFrame, int ** &array);
void stddev(Mat &frame, Mat &gray, Mat& meanFrame, Mat& newFrame, int **&array);
void task2(Mat& frame, Mat& gray, Mat& meanFrame, Mat& newFrame, Mat &finalFrame);
void contours(String image);
void identify(Mat& frame, int &area, int &length);


int main(int argc, char** argv)
{
	int height = 0;
	int width = 0;
	int num = 0;

	importVideo(num, height, width);
	cout << "Frame number: " << num << "\nFrame height: " << height << "\nFrame width: " << width << "\n";

	Mat frame = Mat(height, width, CV_8UC1);
	Mat gray = Mat(height, width, CV_8UC1); // Frame --> Grayscale
	Mat meanFrame = Mat::zeros(height, width, CV_8UC1); // Matrix with all mean values
	Mat newFrame = Mat(height, width, CV_32FC1); // Matrix with all standard dev. values
	Mat finalFrame = Mat(height, width, CV_8UC1); // Final frame with the compared values

	// Making an 2x2 matrix via C++ array def.
	int** array = new int* [height];
	for (int i = 0; i < height; i++)
	{
		array[i] = new int[width];
	}
	// Initialize array to 0
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			array[i][j] = 0;
		}
	}

	mean(frame, gray, meanFrame, array);

	// Initialize array to 0
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			array[i][j] = 0;
		}
	}

	stddev(frame, gray, meanFrame, newFrame, array);
	task2(frame, gray,meanFrame, newFrame, finalFrame);

	/*contours("nut1.bmp");
	contours("nut2.bmp");
	contours("nut3.bmp");
	contours("peg1.bmp");
	contours("peg2.bmp");
	contours("peg3.bmp");
	contours("pipe1.bmp");
	contours("prong1.bmp");
	contours("prong2.bmp");
	contours("prong3.bmp");
	contours("prong4.bmp");
	contours("q1.bmp");
	contours("q2.bmp");
	contours("q3.bmp");
	contours("washer1.bmp");
	contours("washer2.bmp");*/


	// Deallocate array
	for (int i = 0; i < height; i++)
	{
		delete[] array[i];
	}
	delete[] array;

	// Terminate the program
	return 0;
}

void importVideo(int& num, int& height, int& width)
{
	VideoCapture video("belt_bg.wmv");
	if (!video.isOpened())
	{
		cout << "Error opening video stream or file";
		return;
	}

	while (1)
	{
		Mat frame;
		video >> frame;

		if (frame.empty()) break;

		/*imshow("Frame", frame);

		char c = (char) waitKey(1);
		if (c == 27)
			break;*/

	}

	num = video.get(CAP_PROP_POS_FRAMES);
	height = video.get(CAP_PROP_FRAME_HEIGHT);
	width = video.get(CAP_PROP_FRAME_WIDTH);

	/*int test = video.get(CAP_PROP_FORMAT);
	cout << test << "\n";*/

	video.release();
	destroyAllWindows();

}

void mean(Mat& frame, Mat &gray, Mat &newFrame, int ** &array)
{
	int value, newVal;
	int num;

	VideoCapture video("belt_bg.wmv");

	if (!video.isOpened())
	{
		cout << "Error opening video stream or file";
		return;
	}

	while (1)
	{
		video >> frame;
		if (frame.empty()) break;

		cvtColor(frame, gray, COLOR_RGB2GRAY); // This conversion works (I checked with imshow)

		for (int r = 0; r < frame.rows; r++)
		{
			for (int c = 0; c < frame.cols; c++)
			{

				value = gray.at<unsigned char>(r, c);
				newVal = array[r][c];
				newVal = newVal + value;
				array[r][c] = newVal;
			}
		}
	}

	num = video.get(CAP_PROP_POS_FRAMES);

	for (int r = 0; r < newFrame.rows; r++)
	{
		for (int c = 0; c < newFrame.cols; c++)
		{
			value = array[r][c];
			//value = newFrame.at<unsigned char>(r, c);
			newVal = value / (num + 1);
			newFrame.at<unsigned char>(r, c) = newVal;
		}
	}
	video.release();
	destroyAllWindows();
}

void stddev(Mat& frame, Mat& gray, Mat& meanFrame, Mat& newFrame, int**& array)
{
	//cout << "Standard Deviation. Return for now.\n";

	int value, newVal, newVal2;
	int num;

	VideoCapture video("belt_bg.wmv");

	if (!video.isOpened())
	{
		cout << "Error opening video stream or file";
		return;
	}
	
	while (1)
	{
		video >> frame;
		if (frame.empty()) break;
		cvtColor(frame, gray, COLOR_RGB2GRAY);

		for (int r = 0; r < frame.rows; r++)
		{
			for (int c = 0; c < frame.cols; c++)
			{
				value = gray.at<unsigned char>(r, c);
				newVal2 = meanFrame.at<unsigned char>(r, c);
				newVal = (value - newVal2);
				newVal = newVal * newVal;
				newVal2 = array[r][c];
				newVal2 = newVal + newVal2;
				array[r][c] = newVal2;
			}
		}
	}
	num = video.get(CAP_PROP_POS_FRAMES);

	float flt1, flt2;

	for (int r = 0; r < newFrame.rows; r++)
	{
		for (int c = 0; c < newFrame.cols; c++)
		{
			value = array[r][c];
			flt1 = value / (num + 1);
			flt2 = sqrt(flt1);
			newFrame.at<float>(r, c) = flt2;
			//cout << newFrame.at<float>(r, c) << " ";
		}
	}

	video.release();
	destroyAllWindows();

}

void task2(Mat &frame, Mat &gray, Mat &meanFrame, Mat &newFrame, Mat &finalFrame)
{
	int value, newVal, newVal2;
	int area = 0;
	int length = 0;
	float flt1;
	int num, thresh;
	thresh = 7;

	VideoCapture video("belt_fg.wmv");

	if (!video.isOpened())
	{
		cout << "Error opening video stream or file";
		return;
	}

	while (1)
	{
		video >> frame;
		if (frame.empty()) break;
		cvtColor(frame, gray, COLOR_RGB2GRAY);

		// The equation given on the lab sheet
		for (int r = 0; r < meanFrame.rows; r++)
		{
			for (int c = 0; c < meanFrame.cols; c++)
			{
				value = gray.at<unsigned char>(r, c);
				newVal = meanFrame.at<unsigned char>(r, c);
				flt1 = newFrame.at<float>(r, c);
				if (abs(value - newVal) > (thresh * flt1))
				{
					finalFrame.at<unsigned char>(r, c) = 255;
				}
				else {
					finalFrame.at<unsigned char>(r, c) = 0;
				}
			}
		}
		//imshow("Original Video", frame);

		String title = "Background subtractor";
		namedWindow(title, WINDOW_NORMAL);
		imshow(title, finalFrame);

		char c = (char)waitKey(25);
		if (c == 27)
			break;

		identify(finalFrame, area, length);
		cout << "Frame " << video.get(CAP_PROP_POS_FRAMES) << ": ";
		cout << "Contour Area: " << area << ". Arc Length: " << length << ". Identifcation: ";

		// put evaluation identification if statements here
		// NUT
		if ((area > 8500 && area < 10000 && length > 300 && length < 400) ||
			(area > 2000 && area < 2390 && length > 170 && length < 200))
		{
			cout << "NUT" << endl;
		}
		// PEG
		else if (area > 3500 && area < 4600 && length > 200 && length < 270)
		{
			cout << "PEG" << endl;
		}
		// PIPE (I don't like this)
		else if (area > 15 && area < 100 && length > 240 && length < 650)
		{
			cout << "PIPE" << endl;
		}
		// PRONG 
		else if ((area > 8000 && area < 9200 && length > 540 && length < 630) ||
			(area >  15 && area < 50 && length > 1000 && length < 1400))
		{
			cout << "PRONG" << endl;
		}
		// Q
		else if ((area > 2400 && area < 3000 && length > 160 && length < 230) ||
			(area > 6700 && area < 8000 && length > 410 && length < 480))
		{
			cout << "Q" << endl;
		}
		else if (area > 5000 && area < 6500 && length > 230 && length < 320)
		{
			cout << "WASHER" << endl;
		}
		else
		{
			cout << "NOT IDENTIFIED" << endl;
		}
		
	}

	video.release();
	destroyAllWindows();
}

void contours(String image)
{
	int largest_area = 0;
	int largest_area_index = 0;
	//Rect bounding_rect;
	Mat src_img = imread(image, CV_8UC1);
	Mat canny1 = Mat::zeros(src_img.rows, src_img.cols, CV_8UC1);
	Mat canny1_mod = canny1.clone();
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	Canny(src_img, canny1, 100, 255);
	findContours(canny1, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

	int idx = 0;
	for (; idx >= 0; idx = hierarchy[idx][0])
	{
		Scalar color(255, 255, 255);
		drawContours(canny1_mod, contours, idx, color, FILLED, 8, hierarchy);
	}

	cout << image << endl;

	// Contour Area code
	for (int i = 0; i < contours.size(); i++)
	{
		double area0 = contourArea(contours[i], false);
		cout << i << ": Contour area is " << area0;
		double area1 = arcLength(contours[i], true);
		cout << ". Arc Length is " << area1 << ".\n";

		if (area0 > largest_area)
		{
			largest_area = area0;
			largest_area_index = i;
			//bounding_rect = boundingRect(contours[i]);
			cout << " NEW LARGEST CONTOUR AREA" << "\n\n";
		}
		else
		{
			cout << endl;
		}
	//cout << i << ": Contour Area: " << area0 << endl;
	}


	//rectangle(src_img, bounding_rect, Scalar(255, 255, 255), 2, 8, 0);
	//namedWindow("Image", WINDOW_NORMAL);
	//imshow("Image", src_img);
	//waitKey(5000);
	//namedWindow("Components", WINDOW_NORMAL);
	//imshow("Components", canny1_mod);
	//waitKey(5000);

	cout << endl;


}

void identify(Mat& frame, int &area, int &length)
{
	int largest_area = 0;
	int largest_length = 0;
	Mat canny1 = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
	Mat canny1_mod = canny1.clone();
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	Canny(frame, canny1, 100, 255);
	findContours(canny1, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

	/*int idx = 0;
	for (; idx >= 0; idx = hierarchy[idx][0])
	{
		Scalar color(255, 255, 255);
		drawContours(canny1_mod, contours, idx, color, FILLED, 8, hierarchy);
	}*/

	for (int i = 0; i < contours.size(); i++)
	{
		double area0 = contourArea(contours[i], false);
		double area1 = arcLength(contours[i], true);

		if (area0 > largest_area)
		{
			largest_area = area0;
			largest_length = area1;
		}
	}

	area = largest_area;
	length = largest_length;

}
