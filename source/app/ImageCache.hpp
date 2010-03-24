#ifndef APP_IMAGECACHE
#define APP_IMAGECACHE

/**
 * Copyright (c) 2009 - 2010 Brandon Thomas Suit
 * http://possibilistic.org | echelon@gmail.com
 * Code available for use under the LGPL 2.
 * 
 * ImageCache
 *   Managed a named (std::string) map of Cv::Image*.
 *   Assumes ownership and automatically deletes added images when the
 *   ImageCache itself is destroyed. 
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
		 * DTOR.
		 * Destroys all images contained.
		 */
		~ImageCache();

		/**
		 * Determine if the key exists.
		 */
		bool exists(std::string name);

		/**
		 * Get pointer to the image.
		 * This is a shared pointer. It cannot be deleted!
		 */
		Cv::Image* get(std::string name);

		/**
		 * Set an image to the key.
		 * Cannot set if key already exists.
		 * Once an image is set, the ImageCache becomes the owner!
		 */
		void set(std::string name, Cv::Image* img);

		/**
		 * Pop the image from the cache.
		 * This returns ownership to the caller, and the caller must delete.
		 */
		Cv::Image* pop(std::string name);

		/**
		 * Delete the image. 
		 * Client cannot use that image again.
		 */
		void del(std::string name);

		/**
		 * Deletes all the images.
		 * Client cannot use any of these images again.
		 */
		void delAll();

	private:
		/**
		 * Cache of images.
		 */
		std::map<std::string, Cv::Image*> cache;
};
}

#endif
