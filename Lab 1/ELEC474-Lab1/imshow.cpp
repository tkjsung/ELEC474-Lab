#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

const String imgdir = "../../images/" ;
//  const String imgdir = "./" ; // this directory

using namespace std;

int main(int argc, char **argv)
{
    cout << "Displaying image " << imgdir + argv[1] << endl ;   //  need to set programs' command line arguments using Project menu

    Mat img = imread( imgdir + argv[1] ) ;                      //  read image

    namedWindow("image", CV_WINDOW_AUTOSIZE);                   //  create a window
    imshow("image", img);                                       //  display image

    waitKey();                                                  //  wait for keystroke
    return 0;
}
