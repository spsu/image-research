#ifndef BT_IMG_HISTOGRAM
#define BT_IMG_HISTOGRAM

namespace Cv {
	class Image;
}

namespace Img {

/**
 * Create a new histogram image from the source image.
 */
Cv::Image* histogram(Cv::Image* img, int width=256, int height=100, int padTop=2, int testIncr=0);

} 

#endif
