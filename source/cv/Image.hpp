#ifndef BT_Cv_Image
#define BT_Cv_Image

/**
 * Copyright (c) 2009 - 2010 Brandon Thomas Suit
 * http://possibilistic.org | echelon@gmail.com
 * Code available for use under the LGPL 2.
 * 
 * Cv::Image
 *   Wrapper for IplImage and part of the OpenCV Arr->Mat->Img OO Hierarchy.
 *   A class to represent and operate on OpenCV images, as well as a method of 
 *   loading files and converting between GdkPixbufs, etc.
 *    TODO: Work with non-8-bit images uniformly. 
 *    TODO: Move image conversion code elsewhere, specifically:
 *			* V4L2::Frame -> Cv::Image   [Image(V4L2::Frame*)]	
 *			* GdkPixbuf -> Cv::Image   [Image(GdkPixbuf*)]
 * 			* Cv::Image -> GdkPixbuf   [toPixbuf(), destroyPixbufCb()]
 */

#include <cv.h>
#include <highgui.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <string>
#include "Mat.hpp"
#include "Arr.hpp"

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

namespace Cv {
class Image : public Mat
{
	public:

		/**
		 * Normal (Blank image) CTORs.
		 * Creates an image with the width, height, channels, etc. specified.
		 * Created as a 3 channel (8-unsigned bits per channel) by default.
		 * (Common channel numbers are 1 and 3, common depths are 8 and 16.)
		 */
		Image(int width, int height,
			  int numChannels = 3, int depth = IPL_DEPTH_8U, 
			  bool isSigned = false);

		Image(CvSize size, int numChannels = 3, int depth = IPL_DEPTH_8U,
			  bool isSigned = false);

		/**
		 * Copy CTOR.
		 */
		Image(const Image& img);

		/**
		 * File load CTOR.
		 * Import file from filename.
		 */
		Image(std::string filename);

		/**
		 * IplImage wrapper CTOR.
		 * Becomes the owner of the IplImage unless isOwner is false.
		 * TODO: Enforce non-writability of shared copies.
		 */
		Image(IplImage* img, bool shared = false);

		/**
		 * Pixbuf load CTOR.
		 * Converts a GDK pixbuf into an IplImage.
		 * Does not assume ownership of the pixbuf.
		 */
		Image(GdkPixbuf* pixbuf);

		/**
		 * V4L2 Frame CTOR.
		 * Converts a V4L2 Frame (my own abstraction) into an IplImage.
		 * Conversion is immediate, and we do not assume ownership of the frame.
		 * XXX: V4L2::Frame is an abstraction of YUYV images only at present.
		 */
		Image(V4L2::Frame* frame);

		/**
		 * IplImage DTOR.
		 */
		~Image();

		/**
		 * Get a pointer to the underlying IplImage.
		 */
		IplImage* getPtr();

		/**
		 * Get various IplImage properties.
		 */
		int getNumChannels() const;
		int getDepth() const;
		int getWidthStep() const;

		/**
		 * Get image data so we can use it in Python via SWIG. 
		 * We couldn't use a char* since SWIG assumes that is null-terminated. 
		 * XXX: Be very careful with this.
		 * TODO: Does this leak memory?
		 */
		std::string getImageData();

		/**
		 * Determines if image is valid (imgPtr != 0).
		 */
		bool isValid() const;

		/**
		 * Get the pixbuf representation of the IplImage.
		 * Caller must deallocate pixbuf.
		 * XXX: Doesn't work with PyGtk, regardless of SWIG wrappers.
		 */
		GdkPixbuf* toPixbuf();

		/**
		 * Easy pixel access.
		 * TODO: Not as easy as img[y][x].r/g/b though!
		 */
		RgbPix getPix();

	protected:

		/**
		 * Closure for destroying copied IplImage created in getPixbuf()
		 */
		static void destroyPixbufCb(guchar* pixels, gpointer data);
};
}

#endif
