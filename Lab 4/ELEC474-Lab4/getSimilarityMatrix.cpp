
#include <stdio.h>
#include <vector>
#include <math.h>
#include <opencv2/opencv.hpp>

Mat getSimilarityMatrix(  
	const Point2f *ptsA, const Point2f *ptsB, int nPts,
	float &scale, float &theta, float &tx, float &ty)
{
	int rows=2*nPts, cols=5 ;
        Mat A(rows,cols,CV_32FC1) ;
	       
	for (int i=0; i<nPts; i++)
        {
                A.at<float>(2*i,0) = ptsA[i].x ;
                A.at<float>(2*i,1) = -ptsA[i].y ;
                A.at<float>(2*i,2) = 1.0 ;
                A.at<float>(2*i,3) = 0.0 ;
                A.at<float>(2*i,4) = -ptsB[i].x ;

                A.at<float>(2*i+1,0) = ptsA[i].y ;
                A.at<float>(2*i+1,1) = ptsA[i].x ;
                A.at<float>(2*i+1,2) = 0.0 ;
                A.at<float>(2*i+1,3) = 1.0 ;
                A.at<float>(2*i+1,4) = -ptsB[i].y ;
        }

	SVD svd ;
        Mat X ;
        svd.solveZ(A, X) ;

	float sa = X.at<float>(0)/X.at<float>(4) ;
        float sb = X.at<float>(1)/X.at<float>(4) ;
        tx = X.at<float>(2)/X.at<float>(4) ;
        ty = X.at<float>(3)/X.at<float>(4) ;

        scale = (float) sqrt((double) (sa*sa + sb*sb)) ;
        theta = atan2(sb,sa) * 180.0/M_PI ;

        Mat M(2,3,CV_32FC1) ;
	M.at<float>(0,0) = sa ;
	M.at<float>(0,1) = -sb ;
	M.at<float>(0,2) = tx ;
	M.at<float>(1,0) = sb ;
	M.at<float>(1,1) = sa ;
	M.at<float>(1,2) = ty ;

	return(M) ;
}



