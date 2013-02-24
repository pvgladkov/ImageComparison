/* 
 * File:   SIFT.cpp
 * Author: pavel
 * 
 * Created on 24 Февраль 2013 г., 0:12
 */

#include "SIFT.h"
#include<opencv2/core/core.hpp>
#include<Gaussian.h>

SIFT::SIFT() {
}

SIFT::SIFT(const SIFT& orig) {
}

SIFT::~SIFT() {
}

/**
 * 
 * @param img
 * @return 
 */
IplImage* SIFT::greyscale( IplImage* img ){
	
	CvSize sz = { img->width, img->height };
	IplImage* grey = cvCreateImage( sz, img->depth, 1 );
	cvCvtColor( img, grey, CV_RGB2GRAY );
	
	return grey;
}

/**
 * 
 */
void SIFT::processImage(){
	
	// загрузка изображения
	std::string file_path = "C:\\sp\\p.jpg";
	IplImage * img = cvLoadImage( file_path.c_str() );
	if( !img ){
		printf( "Could not load image file: %s\n", file_path.c_str() );
		exit( -1 );
	}

	IplImage* full_color_img = img;
	img = greyscale( full_color_img );

	// get the image data
	int height = img->height;
	int width = img->width;
	int channels = img->nChannels;
	printf("Processing a %dx%d image with %d channels\n",height,width,channels);

	// create a window
	cvNamedWindow( "mainWin", CV_WINDOW_AUTOSIZE );
	cvMoveWindow( "mainWin", 400, 30 );

	int octaves = 4;
	int scales = 5;
	IplImage*** scale_space = generate_scale_space( img, octaves, scales );
	IplImage*** diff_of_gauss =	difference_scale_space( scale_space, octaves, scales );
	IplImage*** keypoint_imgs = keypoint_dog(diff_of_gauss, octaves, scales);

	std::ofstream fout( "C:\\sp\\mc.txt" );
	uchar* sample = (uchar*)diff_of_gauss[3][2]->imageData;
	
	for( int i = 0; i < diff_of_gauss[3][2]->height; ++i ) {
		for( int j = 0; j < diff_of_gauss[3][2]->width; ++j ) {
			fout << (int)sample[i*diff_of_gauss[3][2]->widthStep + j] << " ";
		}
		
		fout << "\n";
	}
	
	fout.flush();
	fout.close();
  
	IplImage * display_image = keypoint_imgs[ 3][0];
	cvShowImage( "mainWin", display_image );

	const char * outfile = "C:\\sp\\out.jpg";
	const char * del_outfile = "DEL /F C:\\sp\\out.jpg";
	std::ifstream fin( outfile );
	if (fin) {
		std::cout << "File found, deleting..." << std::endl;
		system(del_outfile);
		fin.close();
	}

	if( !cvSaveImage( outfile, display_image ) ){
		printf( "Could not save: %s\n", outfile );
	}
	
	// wait for a key
	cvWaitKey(0);

	// release the image
	cvReleaseImage(&img);
}

/**
 * 
 * @param dog
 * @param octaves
 * @param scales
 * @return 
 */
IplImage*** SIFT::keypoint_dog( IplImage *** dog, int octaves, int scales ) {
	
	--scales; // We lost a scale when applying diff of gaussians.
	IplImage *** keypoint_dog = new IplImage**[octaves];
	for( int i = 0; i < octaves; ++i ) {
		keypoint_dog[i] = new IplImage*[scales-2];
		for( int j = 0; j < scales-2; ++j ) {
			CvSize sz = {dog[i][0]->width, dog[i][0]->height};
			keypoint_dog[i][j] = cvCreateImage(sz, dog[i][0]->depth, 1);
		}
	}

	for( int i = 0;i < octaves; ++i ) {
		for( int j = 1;j < scales-1; ++j ){ // Skip first and last

			IplImage* curr_image = dog[i][j];
			uchar* data = (uchar*)curr_image->imageData;
			int height = curr_image->height;
			int width = curr_image->width;
			int width_sz = curr_image->widthStep;
			std::cout << width << " " << width_sz << std::endl;

			uchar* keypoint_data = (uchar*)keypoint_dog[i][j-1]->imageData;
			for( int a = 1; a < height-1; ++a ) {
				for( int b = 1; b < width_sz-1; ++b ) {
					uchar curr_px = data[a*width_sz + b];

					bool max = true;
					bool min = true;
					for( int d = -1; d <= 1; ++d ) {
						for( int dd = -1; dd <= 1; ++dd ) {
							for( int ddd = -1 ; ddd <= 1; ++ddd ) {
								
								if( !( d && dd && ddd ) ){
									continue;
								}
								
								IplImage * curr_test_image = dog[i][j+d];
								uchar * curr_test_data = (uchar*)curr_test_image->imageData;
								if(curr_px < curr_test_data[(a+dd)*width_sz + (b + ddd)]){
									max = false;
								}

								if(curr_px > curr_test_data[(a+dd)*width_sz + (b + ddd)]){
									min = false;
								}
							}
						}
					}

					bool is_keypoint = max || min;

					keypoint_data[a*width_sz + b] = is_keypoint
						? data[a*width_sz + b] 
						: 0
					;
				}
			}
		}
	}
	
	return keypoint_dog;
}

/**
 * 
 * @param img
 * @param octaves
 * @param scales
 * @return 
 */
IplImage*** SIFT::generate_scale_space( IplImage * img, int octaves, int scales ) {
	// scale_space[i][j] is the ith octave, jth scale (blur level)
	IplImage*** scale_space = new IplImage**[octaves];
	for( int i = 0;i < octaves; ++i ) {
		scale_space[i] = new IplImage*[scales];
	}
	scale_space[0][0] = img;

	for( int i = 1;i < octaves; ++i ) {
		IplImage* prev = scale_space[i-1][0];
		int new_height = (int)ceil(prev->height / sqrt(2.0));
		int new_width = (int)ceil(prev->width / sqrt(2.0));

		CvSize new_cvSize = { new_width, new_height };
		IplImage* resized = cvCreateImage(
			new_cvSize, prev->depth, prev->nChannels
		);

		cvResize( prev, resized );
		scale_space[i][0] = resized;
	}

	Gaussian* g = new Gaussian();

	double sigma_step = 1 / sqrt(2.0);
	double k_step = 1 / sigma_step;
	for( int i = 0; i < octaves; ++i ){
		for( int j = 1; j < scales; ++j ) {
			scale_space[i][j] = g->gaussianBlur( scale_space[i][j-1], sigma_step );
		}
		sigma_step *= k_step;
	}

  return scale_space;
}