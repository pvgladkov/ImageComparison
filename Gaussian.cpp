/* 
 * File:   Gaussian.cpp
 * Author: pavel
 * 
 * Created on 27 Январь 2013 г., 17:20
 */

#include "Gaussian.h"
#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#define PI 3.14

Gaussian::Gaussian() {
}

Gaussian::Gaussian(const Gaussian& orig) {
}

Gaussian::~Gaussian() {
}

/**
 * 
 * @param img
 * @param sigma
 * @return 
 */
IplImage* Gaussian::gaussianBlur( IplImage* const img, double sigma ){
	
	// copy image
	IplImage * img_copy = cvCloneImage(img);
	
	uchar * data = (uchar *)img_copy->imageData;
	uchar * ori_data = (uchar *)img->imageData;
	
	int step_sz = img_copy->widthStep;

	const int kernel_sz = (int)ceil(6*sigma);
	const int center = kernel_sz / 2;

	std::cout << "Kernel sz: " << kernel_sz << std::endl;

	double normalization_factor = 2.0*PI*sigma*sigma;
	double ** kernel = new double*[kernel_sz];
	for( int i = 0; i < kernel_sz; ++i ) {
		kernel[i] = new double[kernel_sz];
		for( int j = 0; j < kernel_sz; ++j ) {
			
			double x = std::abs(i-center);
			double y = std::abs(j-center);
			kernel[i][j] = exp(-1.0 * (x*x + y*y) / (2*sigma*sigma));
			kernel[i][j] /= normalization_factor;
		}
	}
	
	int channels = img_copy->nChannels;
	for( int i = 0; i < img_copy->height; ++i ) {

		for( int j = 0; j < img_copy->width; ++j ) {
			for( int k = 0; k < channels; ++k ) {
				
				double kernel_total = 0;
				for( int a = 0; a < kernel_sz; ++a ) {
					for( int b = 0; b < kernel_sz; ++b ) {
						int delta_j = b - center;
						int delta_i = a - center;
						if(
							i + delta_i >= 0 && i + delta_i < img_copy->height &&
							j + delta_j >= 0 && j + delta_j < img_copy->width
						) {
							kernel_total += kernel[a][b];
						}
					}
				}
			
				double compensation = 1.0 / kernel_total;
				double new_value = 0;
				for( int a = 0; a < kernel_sz; ++a ) {
					for( int b = 0; b < kernel_sz; ++b ) {
						int delta_j = b - center;
						int delta_i = a - center;
						if(
							i + delta_i >= 0 && i + delta_i < img_copy->height &&
							j + delta_j >= 0 && j + delta_j < img_copy->width
						) {
							double curr_value = 
								ori_data[(i+delta_i)*step_sz + (j+delta_j)*channels + k];
							new_value += curr_value*kernel[a][b]*compensation;
						}
					}
				}
				data[i*step_sz + j*channels + k] = new_value;
			}
		}
	}
	
	return img_copy;
}