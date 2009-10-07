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
#include <highgui.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <string>

/** 
 * Following canvas code adapted from "Introduction to programming with OpenCV"
 * http://www.cs.iit.edu/~agam/cs512/lect-notes/opencv-intro/opencv-intro.html
 * TODO: Integrate this with main Image class.
 */
template<class T> class Canvas
{
	private:
		IplImage* imgp;
	public:
		Canvas(IplImage* img=0) {imgp=img;}
		~Canvas(){imgp=0;}
		void operator=(IplImage* img) {imgp=img;}
		inline T* operator[](const int rowIndx) {
		return ((T *)(imgp->imageData + rowIndx*imgp->widthStep));}
};

typedef struct{
	unsigned char b,g,r;
} RgbPixel;

typedef Canvas<RgbPixel> RgbImage;

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
		 * Pixbuf load CTOR.
		 * Convert a pixbuf to IplImage.
		 * Does not assume ownership of the pixbuf.
		 */
		Image(GdkPixbuf* pixbuf);

		/**
		 * IplImage DTOR.
		 */
		~Image();

		/**
		 * Get a pointer to the underlying IplImage.
		 */
		IplImage* getPtr();

		/**
		 * Get the pixbuf representation of the IplImage.
		 * Caller must deallocate pixbuf.
		 */
		GdkPixbuf* toPixbuf();

		/**
		 * Get size of the image.
		 */
		int getWidth();
		int getHeight();

		/**
		 * Pixel access.
		 */
		inline RgbPixel* operator[](const int rowIndex) {
			return ((RgbPixel*)(image->imageData + rowIndex*image->widthStep));
		}

	protected:
		/**
		 * OpenCV IplImage.
		 */
		IplImage* image;

		/**
		 * Closure for destroying copied IplImage created in getPixbuf()
		 */
		static void destroyPixbufCb(guchar* pixels, gpointer data);

};
}

#endif
