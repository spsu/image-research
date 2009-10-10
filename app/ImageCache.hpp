#ifndef APP_IMAGECACHE
#define APP_IMAGECACHE

/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 */

#include <string>
#include <map>

namespace Cv {
	class Image;
}

namespace App {
class ImageCache
{
	public:
		/**
		 * CTOR.
		 */
		ImageCache();

		/**
		 * Determine if the key exists.
		 */
		bool exists(std::string name);

		/**
		 * Get pointer to the image.
		 */
		Cv::Image* get(std::string name);

		/**
		 * Set an image to the key.
		 * Cannot set if key already exists.
		 */
		void set(std::string name, Cv::Image* img);

		/**
		 * Pop the image from the cache.
		 */
		Cv::Image* pop(std::string name);

		/**
		 * Delete the image
		 */
		void del(std::string name);

	private:
		/**
		 * Cache of images.
		 */
		std::map<std::string, Cv::Image*> cache;
};
}

#endif
