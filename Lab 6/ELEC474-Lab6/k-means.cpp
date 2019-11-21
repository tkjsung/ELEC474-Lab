
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include "mnist.h"

using namespace cv;
using namespace std;

string trainImage = "train-images.idx3-ubyte";
string trainLabel = "train-labels.idx1-ubyte";
string testImage = "t10k-images.idx3-ubyte";
string testLabel = "t10k-labels.idx1-ubyte";

int main(int, char**)  
{
	// Reading data and labels 
	Mat mnist_Data;
	Mat mnist_labels;
	mnist_Data = read_mnist_image(trainImage);
	mnist_labels = read_mnist_label(trainLabel);
	cout << "Number of rows in DATA: "<<mnist_Data.rows << "     " << "Number of columns in DATA: "<<mnist_Data.cols << endl;
	cout << "Number of rows in LABELS: "<<mnist_labels.rows << "     " << "Number of columns in LABELS: " << mnist_labels.cols << endl;

	int clusterCount=10;
	//Identify the number of clusters
	//clusterCount = ? ;
	Mat centers, labels;
	kmeans(mnist_Data, clusterCount, labels, TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 0.1), 3, KMEANS_PP_CENTERS, centers);
	float count = 0;
	for (int i = 0; i < mnist_Data.rows; i++) {
		if (labels.at<int>(i, 0) == mnist_labels.at<int>(i, 0))
		{
			count++;
			/*to show the correctly categorized image
			Mat img=Mat::zeros(28, 28 , CV_32FC1);
			int ni = 0,nj=0;
			for (int j = 0; j < mnist_Data.cols; j++) {
				if (nj == 28) {
					ni++;
					nj = 0;
				}
				img.at<float>(ni, nj) = mnist_Data.at<float>(i, j)*255;
				nj++;
			}
			cout << "current number: "<< labels.at<int>(i, 0)<<endl;
			imshow("sample", img);
			waitKey();*/
		}
	}
	cout << "correct count = " << count << endl;
	cout << "error rate " << (60000 - count + 0.0) / 60000 * 100.0 << "%....\n";
	return 0;
}
