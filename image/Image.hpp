#ifndef CV_IMAGE
#define CV_IMAGE

/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 * 
 * Class to represent and operate on OpenCV images, as well as a method of 
 * converting to GdkPixbuf.
 */

#include <cv.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <string>

namespace Cv {
class Image
{
	public:
		/**
		 * Default CTOR does nothing.
		 */
		Image();

		/**
		 * File load CTOR.
		 * Import file from filename.
		 */
		Image(std::string filename);

		/**
		 * IplImage DTOR.
		 */
		~Image();

		/**
		 * Get the pixbuf representation of the IplImage.
		 */
		GdkPixbuf* getPixbuf();

	protected:
		/**
		 * OpenCV IplImage.
		 */
		IplImage* image;

};
}

#endif
