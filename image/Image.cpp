#include "Image.hpp"

namespace Cv {

Image::Image():
	image(0)
{
	// Nothing
}

Image::Image(std::string filename):
	image(0)
{
	image = cvLoadImage(filename);
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

	cvCvtColor(image, rgb, CV_BGR2RGB);

	pb = gdk_pixbuf_new_from_data((guchar*)rgb->imageData,
				GDK_COLORSPACE_RGB,
				false,
				rgb->depth,
				rgb->width,
				rgb->widthStep, 
				NULL, 
				NULL
	);

	cvReleaseImage(&rgb);
	return pb;
}

} // end namespace Cv
