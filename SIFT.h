/* 
 * File:   SIFT.h
 * Author: pavel
 *
 * Created on 24 Февраль 2013 г., 0:12
 */

#ifndef SIFT_H
#define	SIFT_H

#include<opencv2/core/core.hpp>

class SIFT {
public:
	SIFT();
	SIFT(const SIFT& orig);
	virtual ~SIFT();
	
	void processImage();
	
private:
	
	/**
	 * 
     * @param img
     * @return 
     */
	IplImage* greyscale(IplImage * img);
	
	IplImage*** keypoint_dog( IplImage *** dog, int octaves, int scales );
	
	IplImage*** generate_scale_space( IplImage * img, int octaves, int scales );
};

#endif	/* SIFT_H */

