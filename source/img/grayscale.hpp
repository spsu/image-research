#ifndef BT_IMG_GRAYSCALE
#define BT_IMG_GRAYSCALE

namespace Cv {
	class Image;
}

namespace Img {

/**
 * Create a grayscale image based on the one supplied.
 * Overwrites the supplied image.
 */
void grayscale(Cv::Image* img);

/**
 * Create a grayscale image based on the one supplied.
 * Copies the supplied image and generates a new one that the caller must free.
 */
Cv::Image* grayscaleCopy(Cv::Image* in);

} // end namespace Img

#endif
