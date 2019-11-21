#include "mnist.h"

double cost_time;
clock_t start_time;
clock_t end_time;

int testNum = 10000;

int reverseInt(int i) {
	unsigned char c1, c2, c3, c4;

	c1 = i & 255;
	c2 = (i >> 8) & 255;
	c3 = (i >> 16) & 255;
	c4 = (i >> 24) & 255;

	return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}

Mat read_mnist_image(const string fileName) {
	int magic_number = 0;
	int number_of_images = 0;
	int n_rows = 0;
	int n_cols = 0;

	Mat DataMat;

	ifstream file(fileName, ios::binary);
	if (file.is_open())
	{

		file.read((char*)&magic_number, sizeof(magic_number));
		file.read((char*)&number_of_images, sizeof(number_of_images));
		file.read((char*)&n_rows, sizeof(n_rows));
		file.read((char*)&n_cols, sizeof(n_cols));

		magic_number = reverseInt(magic_number);
		number_of_images = reverseInt(number_of_images);
		n_rows = reverseInt(n_rows);
		n_cols = reverseInt(n_cols);

		cout << "reading image......\n";
		start_time = clock();
		DataMat = Mat::zeros(number_of_images, n_rows * n_cols, CV_32FC1);
		for (int i = 0; i < number_of_images; i++) {
			for (int j = 0; j < n_rows * n_cols; j++) {
				unsigned char temp = 0;
				file.read((char*)&temp, sizeof(temp));
				float pixel_value = float((temp + 0.0) / 255.0);
				DataMat.at<float>(i, j) = pixel_value;
			}
		}
		end_time = clock();
		cost_time = (end_time - start_time) / CLOCKS_PER_SEC;
		cout << "finish reading......" << cost_time << "s\n";
	}
	file.close();
	return DataMat;
}

Mat read_mnist_label(const string fileName) {
	int magic_number;
	int number_of_items;

	Mat LabelMat;

	ifstream file(fileName, ios::binary);
	if (file.is_open())
	{

		file.read((char*)&magic_number, sizeof(magic_number));
		file.read((char*)&number_of_items, sizeof(number_of_items));
		magic_number = reverseInt(magic_number);
		number_of_items = reverseInt(number_of_items);

		cout << "reading label......\n";
		start_time = clock();
		LabelMat = Mat::zeros(number_of_items, 1, CV_32SC1);
		for (int i = 0; i < number_of_items; i++) {
			unsigned char temp = 0;
			file.read((char*)&temp, sizeof(temp));
			LabelMat.at<int>(i, 0) = (int)temp;
		}
		end_time = clock();
		cost_time = (end_time - start_time) / CLOCKS_PER_SEC;
		cout << "finish reading......" << cost_time << "s\n";
	}
	file.close();
	return LabelMat;
}