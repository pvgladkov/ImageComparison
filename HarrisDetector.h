/* 
 * File:   HarrisDetector.h
 * Author: pavel
 *
 * Created on 8 Январь 2013 г., 19:55
 */

#ifndef HARRISDETECTOR_H
#define	HARRISDETECTOR_H

#include<opencv2/core/core.hpp>

class HarrisDetector {
public:
	HarrisDetector() : neighbourhood(3), aperture(3),
						k(0.01), maxStrength(0.0), threshold(0.01),
						nonMaxSize(3) {
		// создаем ядро
		//setLocalMaxWindowSize( nonMaxSize );
	}
	HarrisDetector(const HarrisDetector& orig);
	virtual ~HarrisDetector();
	
	void detect( const cv::Mat& image );
	
	cv::Mat getCornerMap( double qualityLevel );
	
	void getCorners( std::vector<cv::Point> &points, double qualityLevel);
	
	void getCorners( std::vector<cv::Point> &points, const cv::Mat& cornerMap );
	
private:
	
	// 
	cv::Mat cornerStrength;
	// 
	cv::Mat cornerTh;
	// 
	cv::Mat localMax;
	//
	int neighbourhood;
	//
	int aperture;
	//
	double k;
	//
	double maxStrength;
	//
	double threshold;
	//
	int nonMaxSize;
	//
	cv::Mat kernel;
	//
	void setLocalMaxWindowSize(int);
};

#endif	/* HARRISDETECTOR_H */

