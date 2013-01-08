/* 
 * File:   HarrisDetector.cpp
 * Author: pavel
 * 
 * Created on 8 Январь 2013 г., 19:55
 */

#include <opencv2/imgproc/imgproc.hpp>

#include "HarrisDetector.h"

HarrisDetector::~HarrisDetector() {
}

/**
 * 
 * @param image
 */
void HarrisDetector::detect( const cv::Mat& image ){
	
	//
	cv::cornerHarris( image, cornerStrength, neighbourhood, aperture, k );
	
	//
	double minStrength;
	cv::minMaxLoc(cornerStrength, &minStrength, &maxStrength);
	
	// 
	cv::Mat dilated;
	cv::dilate(cornerStrength, dilated, cv::Mat());
	cv::compare(cornerStrength, dilated, localMax, cv::CMP_EQ);
}

/**
 * 
 * @param qualityLevel
 * @return 
 */
cv::Mat HarrisDetector::getCornerMap( double qualityLevel ){
	
	cv::Mat cornerMap;
	
	threshold = qualityLevel * maxStrength;
	cv::threshold(cornerStrength, cornerTh, threshold, 255, cv::THRESH_BINARY);
	
	cornerTh.convertTo(cornerMap, CV_8U);
	
	cv::bitwise_and(cornerMap, localMax, cornerMap);
	
	return cornerMap;
}

void HarrisDetector::getCorners( std::vector<cv::Point> &points, double qualityLevel){
	
	cv::Mat cornerMap = getCornerMap(qualityLevel);
	getCorners(points, cornerMap);
}

/**
 * 
 * @param points
 * @param cornerMap
 */
void HarrisDetector::getCorners(std::vector<cv::Point>& points, const cv::Mat& cornerMap){
	
	for( int y = 0; y < cornerMap.rows; y++ ){
		const uchar* rowPtr = cornerMap.ptr<u_char>(y);
		for( int x = 0; x < cornerMap.cols; x++ ){
			if( rowPtr[x] ){
				points.push_back(cv::Point(x,y));
			}
		}
	}
}
