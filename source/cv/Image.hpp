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

namespace V4L2 {
	class Frame;
}

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
		 * TODO: Copy parameter, or enum CV_IMAGE_COPY, CV_IMAGE_SHARED, etc...
		 */
		Image(IplImage* img, bool shared = false);

		/**
		 * Copy CTOR.
		 * Copy the image supplied.
		 */
		Image(const Image& c);

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
		 * Converts a GDK pixbuf into an IplImage.
		 * Does not assume ownership of the pixbuf.
		 * TODO XXX: THIS SHOULD NOT BE HERE! Have in an app-specific library. 
		 */
		Image(GdkPixbuf* pixbuf);

		/**
		 * V4L2 Frame CTOR.
		 * Converts a V4L2 Frame (my own abstraction) into an IplImage.
		 * Conversion is immediate, and we do not assume ownership of the frame.
		 * TODO XXX: This should not be here! (V4L2::Frame is an abstraction of 
		 * only YUYV images only at present, nevertheless it needs removal.)
		 */
		Image(V4L2::Frame* frame);

		/**
		 * IplImage DTOR.
		 */
		~Image();

		/**
		 * Return a new copy of the image. (Alternate method to copy CTOR.)
		 * Newly returned copy must be freed by the caller. 
		 */
		Image* copy();

		/**
		 * Get a pointer to the underlying IplImage.
		 */
		IplImage* getPtr();

		/**
		 * Determines if image is valid (imgPtr != 0).
		 * TODO: Is this needed?
		 */
		bool isValid();

		/**
		 * Get the pixbuf representation of the IplImage.
		 * Caller must deallocate pixbuf.
		 * XXX TODO: This should not be here! Have in app-specific library. 
		 */
		GdkPixbuf* toPixbuf();

		/**
		 * Get size of the image.
		 */
		CvSize getSize();
		int getWidth();
		int getHeight();

		/**
		 * Easy pixel access.
		 * TODO: Not as easy as img[y][x].r/g/b though!
		 */
		RgbPix getPix();

		/**
		 * Resize the current image.
		 * Creating a copy of the image first might be a good idea.
		 */
		void resize(int width, int height);

		/**
		 * Resize both dimensions by a floating point scale factor.
		 * Creating a copy of the image first might be a good idea.
		 */
		void resize(double factor);

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
		 * TODO: Remove when toPixbuf() is removed. 
		 */
		static void destroyPixbufCb(guchar* pixels, gpointer data);
};
}

#endif
