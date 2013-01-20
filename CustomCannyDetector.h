/* 
 * File:   CustomCannyDetector.h
 *
 * Created on 20 Январь 2013 г., 19:50
 */

#ifndef CUSTOMCANNYDETECTOR_H
#define	CUSTOMCANNYDETECTOR_H

#define PI = 3.14

typedef unsigned char uint8_t;

class CustomCannyDetector {
public:
	CustomCannyDetector();
	CustomCannyDetector(const CustomCannyDetector& orig);
	virtual ~CustomCannyDetector();
	
	cv::Mat processImage( const cv::Mat& image );
private:

	/**
	 * \var Bitmap with source image.
	 */
	uint8_t* source_bitmap;

	/**
	 * \var Bitmap with image that algorithm is working on.
	 */
	uint8_t* workspace_bitmap;

	/**
	 * \var Array storing gradient magnitude.
	 */
	float* edge_magnitude;

	/**
	 * \var Array storing edge direction (0, 45, 90 and 135 degrees).
	 */
	uint8_t* edge_direction;

	/**
	 * \var Width of currently processed image, in pixels.
	 */
	unsigned int width;

	/**
	 * \var Height of currently processed image, in pixels.
	 */
	unsigned int height;

	/**
	 * \var Pixel x coordinate.
	 */
	unsigned int x;

	/**
	 * \var Pixel y coordinate.
	 */
	unsigned int y;

	/**
	 * \var Width of Gauss transform mask (kernel).
	 */
	unsigned int mask_size;

	/**
	 * \var Width of the margin (floor of half of the Gauss mask size).
	 */
	unsigned int mask_halfsize;

	/**
	 * \brief Gets value of (x, y) pixel.
	 *
	 * Operates only on `workspace_bitmap`.
	 *
	 * \param x Pixel x coordinate.
	 * \param y Pixel y coordinate.
	 * \return Pixel (x, y) value.
	 */
	inline uint8_t GetPixelValue(unsigned int x, unsigned int y);

	/**
	 * \brief Sets (x, y) pixel to certain value.
	 *
	 * Operates only on `workspace_bitmap`.
	 *
	 * \param x Pixel x coordinate.
	 * \param y Pixel y coordinate.
	 * \param value Pixel value (0-255).
	 */
	inline void SetPixelValue(unsigned int x, unsigned int y, uint8_t value);

	/**
	 * \brief Initializes arrays for use by the algorithm.
	 *
	 * \param sigma Parameter used for calculation of margin that the image
	 * must be enlarged with.
	 */
	void PreProcessImage(float sigma);

	/**
	 * \brief Cuts margins and returns image of original size.
	 */
	void PostProcessImage();

	/**
	 * \brief Converts image to grayscale.
	 *
	 * Information of chrominance are useless, we only need grayscale image.
	 */
	void Luminance();

	/**
	 * \brief Convolves image with Gauss filter - performs Gaussian blur.
	 *
	 * This step performs noise reduction algorithm.
	 *
	 * \param sigma Gaussian function standard deviation. The higher value,
	 * the stronger blur.
	 */
	void GaussianBlur(float sigma);

	/**
	 * \brief Calculates magnitude and direction of image gradient.
	 *
	 * Method saves results in two arrays, edge_magnitude and
	 * edge_direction.
	 */
	void EdgeDetection();

	/**
	 * \brief Deletes non-max pixels from gradient magnitude map.
	 *
	 * By using edge direction information this method looks for local
	 * maxima of gradient magnitude. As a result we get map with edges
	 * of 1 pixel width.
	 */
	void NonMaxSuppression();

	/**
	 * \brief Performs hysteresis thresholding between two values.
	 *
	 * \param lowThreshold Lower threshold of hysteresis (from range of 0-255).
	 * \param highThreshold Upper threshold of hysteresis (from range of 0-255).
	 */
	void Hysteresis(uint8_t lowThreshold, uint8_t highThreshold);

	/**
	 * \brief Support method in hysteresis thresholding operation.
	 *
	 * \param x Pixel x coordinate.
	 * \param y Pixel y coordinate.
	 * \param lowThreshold Lower threshold of hysteresis (from range of 0-255).
	 */
	void HysteresisRecursion(long x, long y, uint8_t lowThreshold);
};

#endif	/* CUSTOMCANNYDETECTOR_H */

