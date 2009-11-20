#include "Image.hpp"
#include "../v4l2/easyapi/Frame.hpp" // TODO: Allow compilation without this...
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

	memcpy(image->imageData, gdk_pixbuf_get_pixels(pixbuf), copyBytes); // TODO 
}

Image::Image(V4L2::Frame* frame):
	image(0),
	isOwner(true)
{
	const unsigned char* p = 0;	// src
	int len = 0;

	int width = frame->getWidth();
	int height = frame->getHeight();

	image = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

	p = frame->getData();
	len = frame->getBytesUsed();

	RgbPix pix = getPix();

	// For YUYV information:
	// http://v4l2spec.bytesex.org/spec/r4339.htm
	// http://www.fourcc.org/fccyvrgb.php
	int j = 0;
	int x = 0;
	int y = 0;
	for(int i = 0; i < len; i+=4) 
	{
		int y1 = p[i];
		int cb = p[i+1];
		int y2 = p[i+2];
		int cr = p[i+3];

		// Conversion formula for YUV colorspace to RGB colorspace
		// R = 1.164(Y - 16) + 1.596(V - 128)
		// G = 1.164(Y - 16) - 0.813(V - 128) - 0.391(U - 128)
		// B = 1.164(Y - 16)                  + 2.018(U - 128)
		
		int r1 = (int) 1.164*(y1 - 16) + 1.596*(cr - 128);
		int g1 = (int) 1.164*(y1 - 16) - 0.813*(cr - 128) - 0.391*(cb - 128);
		int b1 = (int) 1.164*(y1 - 16) + 2.018*(cb - 128);
		
		int r2 = (int) 1.164*(y2 - 16) + 1.596*(cr - 128);
		int g2 = (int) 1.164*(y2 - 16) - 0.813*(cr - 128) - 0.391*(cb - 128);
		int b2 = (int) 1.164*(y2 - 16) + 2.018*(cb - 128);

		// There has to be a faster to truncate out of bounds values.
		// Perhaps bitwise... ?
		if(r1 > 255) {
			r1 = 255;
		}
		if(g1 > 255) {
			g1 = 255;
		}
		if(b1 > 255) {
			b1 = 255;
		}
		if(r2 > 255) {
			r2 = 255;
		}
		if(g2 > 255) {
			g2 = 255;
		}
		if(b2 > 255) {
			b2 = 255;
		}

		if(r1 < 0) {
			r1 = 0;
		}	
		if(g1 < 0) {
			g1 = 0;
		}	
		if(b1 < 0) {
			b1 = 0;
		}	
		if(r2 < 0) {
			r2 = 0;
		}	
		if(g2 < 0) {
			g2 = 0;
		}	 
		if(b2 < 0) {
			b2 = 0;
		}

		// IplImage is BGR. 
		/*// XXX: Is this code causing segfaults for users of the IplImage??
		image->imageData[j] = (int)b1;
		image->imageData[j+1] = (int)g1;
		image->imageData[j+2] = (int)r1;

		image->imageData[j+3] = (int)b2;
		image->imageData[j+4] = (int)g2;
		image->imageData[j+5] = (int)r2;*/ 

		pix[y][x].r = (int)r1;
		pix[y][x].g = (int)g1;
		pix[y][x].b = (int)b1;

		x += 1;
		if(x >= width) {
			x = 0;
			y += 1;
		}

		pix[y][x].r = (int)r2;
		pix[y][x].g = (int)g2;
		pix[y][x].b = (int)b2;

		x += 1;
		if(x >= width) {
			x = 0;
			y += 1;
		}

		j += 6;
	}
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

bool Image::isValid()
{
	return bool(image != 0);
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
