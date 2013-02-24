/* 
 * File:   Ransac.h
 * Author: pavel
 *
 * Created on 4 Февраль 2013 г., 23:26
 */

#ifndef RANSAC_H
#define	RANSAC_H

#include<vector>
#include<opencv2/core/core.hpp>

using namespace cv;

class Ransac {
public:
	Ransac();
	Ransac(const Ransac& orig);
	virtual ~Ransac();
	
	void circle( 
		Mat &image,
		std::vector<Vec3f> &circles,
		double canny_threshold,
		double circle_threshold,
		int numIterations 
	);
	
private:

};

#endif	/* RANSAC_H */

