#include "Image.hpp"
#include <stdio.h>

namespace Cv {

Image::Image():
	image(0),
	isOwner(true)
{
	// TODO
}

Image::Image(IplImage* img, bool shared):
	image(0),
	isOwner(!shared)
{
	image = img;
}

Image::Image(int width, int height, int numChannels):
	image(0),
	isOwner(true)
{
	image = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, numChannels);
}

Image::Image(std::string filename):
	image(0),
	isOwner(true)
{
	image = cvLoadImage(filename.c_str(), CV_LOAD_IMAGE_COLOR);
}

Image::Image(GdkPixbuf* pixbuf):
	image(0),
	isOwner(true)
{
	//CvSize sz;
	int width = 0;
	int height = 0;
	int bitsPerSamp = 0;
	int numChannels = 0;
	int copyBytes = 0;

	if(pixbuf == NULL) {
		// TODO
	}

	width = gdk_pixbuf_get_width(pixbuf);
	height = gdk_pixbuf_get_height(pixbuf);
	bitsPerSamp = gdk_pixbuf_get_bits_per_sample(pixbuf);
	numChannels = gdk_pixbuf_get_n_channels(pixbuf);

	//printf("Width: %d, height: %d, bits: %d, channels: %d\n", width, height, 
	//			bitsPerSamp, numChannels);

	// TODO: Catch errors.
	//sz = cvSize(gdk_pixbuf_get_width(pixbuf), gdk_pixbuf_get_height(pixbuf));
	//image = cvCreateImageHeader(cvSize(width, height), 8, numChannels);
	image = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, numChannels);
	
	//image->imageData = gdk_pixbuf_get_pixels(pixbuf);

	// For pixbuf memory details, refer to:
	// http://library.gnome.org/devel/gdk-pixbuf/stable/gdk-pixbuf-gdk-
	// pixbuf.html#image-data
	copyBytes = (height-1) * gdk_pixbuf_get_rowstride(pixbuf);
	copyBytes += width * ((numChannels * bitsPerSamp + 7) / 8); // final row

	memcpy(image->imageData, gdk_pixbuf_get_pixels(pixbuf), copyBytes); // TODO TEST
}

Image::~Image()
{
	if(image != NULL && isOwner) {
		cvReleaseImage(&image);
	}
}

IplImage* Image::getPtr()
{
	return image;
}

GdkPixbuf* Image::toPixbuf()
{
	IplImage* rgb = 0;
	GdkPixbuf* pb = 0;

	rgb = cvCreateImage(cvGetSize(image), 8, 3);
	cvCopyImage(image, rgb);
	cvCvtColor(rgb, rgb, CV_BGR2RGB);

	pb = gdk_pixbuf_new_from_data((guchar*)rgb->imageData,
				GDK_COLORSPACE_RGB,
				false,
				rgb->depth,
				rgb->width,
				rgb->height,
				rgb->widthStep, 
				destroyPixbufCb, // Closure to destroy IplImage*
				rgb				 // Closure data (the IplImage*)
	);

	return pb;
}

int Image::getWidth()
{
	if(image == NULL) {
		return 0;
	}
	return image->width;
}

int Image::getHeight()
{
	if(image == NULL) {
		return 0;
	}
	return image->height;
}

RgbPix Image::getPix()
{
	return RgbPix(image);
}

// Closure for destroying copy IplImages used in creating pixbufs
void Image::destroyPixbufCb(guchar* pixels, gpointer data)
{
	//printf("Closure destroying pixbuf's IplImage*\n");
	IplImage* copy = (IplImage*)data;
	cvReleaseImage(&copy);
}

} // end namespace Cv
