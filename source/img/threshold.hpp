#ifndef BT_IMG_THRESHOLD
#define BT_IMG_THRESHOLD

#include <vector>
#include "Color.hpp"

namespace Cv {
	class Image;
}

namespace Img {

/**
 * TODO
 * Perform iterative thresholding on the image.
 **/
void threshold(Cv::Image* img, int numRegions=2); //, std::vector<Color> colors = std::vector<Color>() );

/**
 * Perform a binary threshold on the image.
 * This segments the image into black and white regions
 * using iterative thresholding.
 **/
void binaryThreshold(Cv::Image* img);

/**
 * TODO
 * Perform iterative thresholding on a copy of the image.
 */
Cv::Image* thresholdCopy(Cv::Image* img, int numRegions=2);

/**
 * Make a binary threshold copy of the image.
 * This segments the image into black and white regions
 * using iterative thresholding.
 */
Cv::Image* binaryThresholdCopy(Cv::Image* img);

}

#endif
