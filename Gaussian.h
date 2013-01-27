/* 
 * File:   Gaussian.h
 * Author: pavel
 *
 * Created on 27 Январь 2013 г., 17:20
 */

#ifndef GAUSSIAN_H
#define	GAUSSIAN_H
#include <opencv2/core/core.hpp>

class Gaussian {
public:
	Gaussian();
	Gaussian(const Gaussian& orig);
	virtual ~Gaussian();
	
	IplImage* gaussianBlur( IplImage* const image, double sigma );
private:

};

#endif	/* GAUSSIAN_H */

