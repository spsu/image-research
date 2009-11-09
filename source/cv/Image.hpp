#ifndef CV_IMAGE
#define CV_IMAGE

/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
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
typedef Canvas<RgbPixel> RgbPix;

/**
 * Cv::Image
 * A class to represent and operate on OpenCV images, as well as a method of 
 * loading files and converting between GdkPixbufs, etc.
 * Only represents 8-channel BGR images for now. I'll have to generalize this in
 * order to use it with other OpenCV-related code. 
 */
namespace Cv {
class Image
{
	public:
		/**
		 * Default CTOR does nothing.
		 */
		Image();

		/**
		 * IplImage wrapper CTOR.
		 * Becomes the owner of the IplImage unless isOwner is false.
		 * TODO: Enforce non-writability of shared copies.
		 */
		Image(IplImage* img, bool shared = false);

		/**
		 * Blank image CTOR.
		 * Creates an image with the width, height, channels, etc. specified.
		 * Created with 8-bits per channel, unsigned.
		 */
		Image(int width, int height, int numChannels = 3);

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
		 * Determines if image is valid (imgPtr != 0).
		 */
		bool isValid();

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
		 * Easy pixel access.
		 * TODO: Not as easy as img[y][x].r/g/b though!
		 */
		RgbPix getPix();

	protected:
		/**
		 * OpenCV IplImage.
		 */
		IplImage* image;

		/**
		 * Whether the Image class owns the IplImage.
		 */
		bool isOwner;

		/**
		 * Closure for destroying copied IplImage created in getPixbuf()
		 */
		static void destroyPixbufCb(guchar* pixels, gpointer data);
};
}

#endif
