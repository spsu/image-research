#ifndef BT_IMG_THRESHOLD
#define BT_IMG_THRESHOLD

namespace Cv {
	class Image;
}

namespace Img {

/**
 * Segment an image into distinct regions.
 * Uses the iterative thresholding algorithm.
 * Grayscale colors are used. 
 */
void threshold(Cv::Image* img, int numRegions=2);

/**
 * Copy the image and segment it into distinct regions.
 * Uses the iterative thresholding algorithm.
 * Grayscale colors are used. 
 */
Cv::Image* thresholdCopy(Cv::Image* img, int numRegions=2);

}

#endif
