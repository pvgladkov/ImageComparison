/* 
 * File:   CannyDetector.cpp
 * 
 * Created on 20 Январь 2013 г., 16:56
 */

#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "CannyDetector.h"


using namespace cv;

CannyDetector::CannyDetector(): threshold1(10),
								threshold2(100),
								apertureSize(3) {
}

CannyDetector::CannyDetector(const CannyDetector& orig) {
}

CannyDetector::~CannyDetector() {
}

void CannyDetector::printEdges( const Mat& image ){
	
	Mat dst;

	cvNamedWindow( "cvCanny",CV_WINDOW_AUTOSIZE );
	
	cv::Canny( image, dst, threshold1, threshold2, apertureSize );

	imshow( "cvCanny", dst );
	
	cvWaitKey(0);
}

