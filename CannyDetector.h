/* 
 * File:   CannyDetector.h
 *
 * Created on 20 Январь 2013 г., 16:56
 */

#ifndef CANNYDETECTOR_H
#define	CANNYDETECTOR_H

class CannyDetector {
public:
	CannyDetector();
	CannyDetector(const CannyDetector& orig);
	virtual ~CannyDetector();
	
	void printEdges(  const cv::Mat& image );
private:
	double threshold1;
	double threshold2;
	int apertureSize;
};

#endif	/* CANNYDETECTOR_H */

