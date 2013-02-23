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


IplImage * difference(IplImage * img1, IplImage * img2) {
	
	int height = img1->height;
	int width = img1->width;
	assert(height == img2->height && width == img2->width);

	uchar * data1 = (uchar*)img1->imageData;
	uchar * data2 = (uchar*)img2->imageData;

	CvSize sz = {width, height};
	IplImage * difference = cvCreateImage(sz, img1->depth, 1);
	uchar * diff_data = (uchar*)difference->imageData;
	int width_step = img1->widthStep;


	for(int i=0;i<height;++i) {
		for(int j=0;j<width;++j) {
			int curr_px = i*width + j;
			diff_data[curr_px] = abs(data1[curr_px] - data2[curr_px]);
			if(diff_data[curr_px] < 16)
			  diff_data[curr_px] *= diff_data[curr_px];
			else
			  diff_data[curr_px] = 255;

			if(diff_data[curr_px] < 10)
			  diff_data[curr_px] = 0;
		}
	}

	return difference;
}

IplImage ** difference_octave(IplImage ** octave, int scales) {
	
	IplImage ** dog_octave = new IplImage*[scales-1];
	for(int i=0;i<scales-1;++i){
		dog_octave[i] = difference(octave[i], octave[i+1]);
	}

	return dog_octave;
}

IplImage *** difference_scale_space(IplImage *** scale_space,
                                    int octaves,
                                    int scales)
{

	IplImage *** dog = new IplImage**[octaves];
	for(int i=0;i<octaves;++i){
		dog[i] = difference_octave(scale_space[i], scales);
		std::cout << "difference of gaussian done for octave " << i+1 << std::endl;
	}
	return dog;
}