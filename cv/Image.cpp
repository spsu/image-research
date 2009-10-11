#include "Image.hpp"
#include <stdio.h>

namespace Cv {

Image::Image():
	image(0)
{
	// Nothing
}

Image::Image(int width, int height, int numChannels):
	image(0)
{
	image = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, numChannels);
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
	if(image != NULL) {
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

/*int* Image::pix(int i, int j, int k)
{
	//int step = image->widthStep/sizeof(uchar);
	//int chan = image->nChannels;
	//char* data = image->imageData;
	//return data[i*step + j*chan + k];
//((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 0]
//((uchar*)(img->imageData + img->widthStep*y))[x*3]
	return (int*) &((char*)(image->imageData + image->widthStep*j))[i*3];
	//return ((char*)(image->imageData + i * image->widthStep))[j*image->nChannels + k];
}*/

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
