
#include "mnist.h"

using namespace std;
using namespace cv;
using namespace cv::ml;

string trainImage = "train-images.idx3-ubyte";
string trainLabel = "train-labels.idx1-ubyte";
string testImage = "t10k-images.idx3-ubyte";
string testLabel = "t10k-labels.idx1-ubyte";

double cost_time_;
clock_t start_time_;
clock_t end_time_;

int main()
{
	//--------------------- 1. Set up training data ---------------------------------------
	Mat trainData;
	Mat labels;
	trainData = read_mnist_image(trainImage);
	labels = read_mnist_label(trainLabel);
	cout << "Number of rows in DATA: " << trainData.rows << "     " << "Number of columns in DATA: " << trainData.cols << endl;
	cout << "Number of rows in LABELS: " << labels.rows << "     " << "Number of columns in LABELS: " << labels.cols << endl;

	//------------------------ 2. Set up the support vector machines parameters --------------------
	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::RBF);
	//svm->setDegree(10.0);
	svm->setGamma(0.01);
	//svm->setCoef0(1.0);
	svm->setC(10.0);
	//svm->setNu(0.5);
	//svm->setP(0.1);
	svm->setTermCriteria(TermCriteria(cv::TermCriteria::EPS, 1000, FLT_EPSILON));

	//------------------------ 3. Train the svm ----------------------------------------------------
	cout << "Starting training process" << endl;
	start_time_ = clock();
	svm->train(trainData, ROW_SAMPLE, labels);
	end_time_ = clock();
	cost_time_ = (end_time_ - start_time_) / CLOCKS_PER_SEC;
	cout << "Finished training process...cost " << cost_time_ << " seconds..." << endl;

	//------------------------ 4. save the svm ----------------------------------------------------
	svm->save("mnist_svm.xml");
	cout << "save as mnist_svm.xml" << endl;


	//------------------------ 5. load the svm ----------------------------------------------------
	Ptr<SVM> svm1 = StatModel::load<SVM>("mnist_svm.xml");
	Mat test_data = imread("test.jpg");
	test_data.convertTo(test_data, CV_32F, 1.0 / 255.0);
	resize(test_data, test_data, Size(28, 28));
	Mat test(Size(784, 1), CV_32F);
	for (int i = 0; i < 28; ++i)
		for (int j = 0; j < 28; ++j)
			test.at<float>( 0,i * 28 + j) = test_data.at<float>(i, j);
	cout << svm1->predict(test) << endl;


	//------------------------ 6. read the test dataset -------------------------------------------
	Mat testData;
	Mat tLabel;
	testData = read_mnist_image(testImage);
	tLabel = read_mnist_label(testLabel);
	float count = 0;
	for (int i = 0; i < testData.rows; i++) {
		Mat sample = testData.row(i);
		float res = svm1->predict(sample);
		if (res == tLabel.at<int>(i, 0))
			count++;
	}
	cout << "correct count = " << count << endl;
	cout << "error rate " << (10000 - count + 0.0) / 10000 * 100.0 << "%....\n";
	return 0;
}
