#ifndef BT_IMG_RESIZE
#define BT_IMG_RESIZE

/**
 * Utilize OpenCV algorithms to resize images.
 */

namespace Cv {
	class Image;
}

namespace Img {

/**
 * Create a resized copy of an image that conforms to the diminsions provided.
 */
Cv::Image* resize(Cv::Image* src, int width, int height);

/**
 * Create a resized copy of an image that maintains the original scale,
 * fitting to the new box requirements.
 */
Cv::Image* resize_to_scale(Cv::Image* src, int width, int height);

} 

#endif
