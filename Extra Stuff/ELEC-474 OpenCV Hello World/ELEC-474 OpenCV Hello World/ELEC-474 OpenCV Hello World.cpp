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

// C++ Function Declaration
void opencvHelloWorld();

int main()
{

    std::cout << "Hello World!\n";

	// Call OpenCV Hello World
	opencvHelloWorld();

	// Terminate the program
	return 0;
}

void opencvHelloWorld() {
	// Create an image 
	cv::Mat image;

	// Create 100x100 image of zeros
	//image = cv::Mat::zeros(100, 100, CV_16UC3);

	// Import an image from an JPG
	image = imread("img10.jpg", cv::IMREAD_UNCHANGED);

	std::cout << "Image Size = " << image.cols << "," << image.rows << std::endl;

	// [Optional] Create a display window
	//namedWindow("Display Window", CV_);

	// Display the image
	cv::imshow("Display Window", image);

	// Wait for a keystroke in the window
	// NOTE! If you don't add this the window will close immediately!
	cv::waitKey();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
