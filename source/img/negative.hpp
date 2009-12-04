#ifndef BT_IMG_NEGATIVE
#define BT_IMG_NEGATIVE

namespace Cv {
	class Image;
}

namespace Img {

/**
 * Create a negative image based on the one supplied.
 * Overwrites the supplied image.
 */
void negative(Cv::Image* img);

/**
 * Create a negative image based on the one supplied.
 * Copies the supplied image and generates a new one that the caller must free.
 */
Cv::Image* negativeCopy(Cv::Image* in);


} // end namespace Img

#endif
