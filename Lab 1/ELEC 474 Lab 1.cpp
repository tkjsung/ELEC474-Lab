// ELEC-474 OpenCV Hello World.cpp : This file contains the 'main' function. Program execution begins and ends there.

// C++ Standard Libraries
#include <iostream>

// OpenCV Imports
#include <opencv2/core/core.hpp> // OpenCV Core Functionality
#include <opencv2/highgui/highgui.hpp> // High-Level Graphical User Interface 

// [Optional] Use OpenCV namespace
// NOTE! If note included, all OpenCV functions will need to be prepended with "cv::"
// EX: "Mat image;" -> "cv:Mat image;"
using namespace cv;
using namespace std;

// C++ Function Declaration
//void opencvHelloWorld();
void part1(String picture);
void part2(String picture);
void part3(String picture);
void part4();
void part5();

const String imgdir = "./lena2.jpg";
//  const String imgdir = "./" ; // this directory

int main(int argc, char** argv)
{

    //std::cout << "Hello World!\n";

	part1("lena2.jpg");
	part2("lena2.jpg");
	part3("lena2.jpg");
	part4();
	part5();

	// Terminate the program
	return 0;
}

void part1(String picture)
{
	Mat img = imread(picture);
	namedWindow("Image", WINDOW_AUTOSIZE);
	imshow("Image", img);
	waitKey(3);

	int min = 256;
	int max = 0;
	int avg = 0;
	int count = 0;
	int sum = 0;

	for (int r = 0; r < img.rows; r++)
	{
		for (int c = 0; c < img.cols; c++)
		{
			int val = 0;
			val = img.at<unsigned char>(r, c);
			if (val < min)
			{
				min = val;
			}
			if (val > max)
			{
				max = val;
			}
			count++;
			sum = sum + val;
		}
	}
	avg = sum / count;
	std::cout << "Min:" << min << "\n";
	std::cout << "Max:" << max << "\n";
	std::cout << "Avg:" << avg << "\n";

}

void part2(String picture)
{
	Mat img = imread(picture, cv::IMREAD_GRAYSCALE);
	Mat invertedImg = img.clone();

	for (int r = 0; r < img.rows; r++)
	{
		for (int c = 0; c < img.cols; c++)
		{
			int val = 0;
			val = img.at<unsigned char>(r, c);
			int newval = 255 - val;
			invertedImg.at<unsigned char>(r, c) = newval;
		}
	}

	namedWindow("Image", WINDOW_AUTOSIZE);
	imshow("Image", invertedImg);
	waitKey(10000);
}

void part3(String picture)
{
	Mat img = imread(picture, cv::IMREAD_GRAYSCALE);
	Mat yImg = img.clone();
	Mat xImg = Mat(img.rows,img.cols,cv::IMREAD_GRAYSCALE);

	int row = img.rows;
	int column = img.cols;

	// row is indexed first.
	// In part 3, we want to have mirror reflection in the y-axis, then x-axis
	for (int r = 0; r < img.rows; r++)
	{
		for (int c = 0; c < img.cols; c++)
		{
			int mod1 = column - c - 1;
			int mod2 = row - r - 1;
			int val = 0;
			val = img.at<unsigned char>(r, c);
			yImg.at<unsigned char>(r, mod1) = val;
			xImg.at<unsigned char>(mod2, c) = val;
		}
	}

	namedWindow("Y-axis Reflected Image", WINDOW_AUTOSIZE);
	imshow("Y-axis Reflected Image", yImg);
	waitKey(10000);
	imshow("X-axis Reflected Image", xImg);
	waitKey(10000);
}

void part4() {
	Mat img1 = imread("octo1.jpg", cv::IMREAD_GRAYSCALE);
	Mat img2 = imread("octo2.jpg", cv::IMREAD_GRAYSCALE);
	Mat newImg = img1.clone();
	// 1. Load and display two images of the same dimension
	namedWindow("Image 1", WINDOW_AUTOSIZE);
	imshow("Image 1", img1);
	namedWindow("Image 2", WINDOW_AUTOSIZE);
	imshow("Image 2", img2);
	waitKey(3000);

	// 2. Create and display new image, which is a merger of two input images
	// Note: Do logical OR to find higher value

	for (int r = 0; r < img1.rows; r++) {
		for (int c = 0; c < img1.cols; c++) {
			int val1 = 0;
			int val2 = 0;
			int newval = 0;
			val1 = img1.at<unsigned char>(r, c);
			val2 = img2.at<unsigned char>(r, c);
			if (val1 > val2) {
				newval = val1;
			}
			else {
				newval = val2;
			}
			newImg.at<unsigned char>(r, c) = newval;
		}
	}
	namedWindow("Merged Image", WINDOW_AUTOSIZE);
	imshow("Merged Image", newImg);
	waitKey(10000);
}

void part5() {
	Mat img1 = imread("messy_desk_A-1.jpg",cv::IMREAD_GRAYSCALE);
	Mat img2 = imread("messy_desk_B-1.jpg",cv::IMREAD_GRAYSCALE);
	Mat newImg = Mat(img1.rows,img1.cols,cv::IMREAD_GRAYSCALE);
	
	for (int r = 0; r < img1.rows; r++) {
		for (int c = 0; c < img1.cols; c++) {
			int val1 = 0;
			int val2 = 0;
			val1 = img1.at<unsigned char>(r, c);
			val2 = img2.at<unsigned char>(r, c);
			int difference = 0;
			int tolerance = 5;
			// I should probably add in pixel tolerance in here.
			// Take the difference between val1 and val2
			difference = abs(val1 - val2);

			if (difference > tolerance) {
				newImg.at<unsigned char>(r, c) = 255;
				//std::cout << "Different. Val1:" << val1 << " Val2: " << val2 << "\n";
			}
			else {
				newImg.at<unsigned char>(r, c) = 0;
				//std::cout << "Same. Val1:" << val1 << "Val2: " << val2 << "\n";
			}
		}
	}
	namedWindow("Binary Image", WINDOW_KEEPRATIO);
	imshow("Binary Image", newImg);
	waitKey(10000);
}
