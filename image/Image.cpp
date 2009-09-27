#include "Image.hpp"
#include <stdio.h>

namespace Cv {

Image::Image():
	image(0)
{
	// Nothing
}

Image::Image(std::string filename):
	image(0)
{
	image = cvLoadImage(filename.c_str(), CV_LOAD_IMAGE_COLOR);
}

Image::Image(GdkPixbuf* pixbuf):
	image(0)
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

	// TODO: Catch errors.
	//sz = cvSize(gdk_pixbuf_get_width(pixbuf), gdk_pixbuf_get_height(pixbuf));
	image = cvCreateImageHeader(cvSize(width, height), 8, numChannels);
	
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
	if(image != NULL) {
		cvReleaseImage(&image);
	}
}

GdkPixbuf* Image::getPixbuf()
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

// Closure for destroying copy IplImages used in creating pixbufs
void Image::destroyPixbufCb(guchar* pixels, gpointer data)
{
	printf("Closure destroying pixbuf's IplImage*\n");
	IplImage* copy = (IplImage*)data;
	cvReleaseImage(&copy);
}

} // end namespace Cv
