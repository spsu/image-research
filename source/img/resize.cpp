#include "resize.hpp"
#include "../cv/Image.hpp"

#include <cv.h>
#include <math.h>

namespace Img {

Cv::Image* resize(Cv::Image* img, int width, int height)
{
	Cv::Image* out = new Cv::Image(width, height);
	cvResize(img->getPtr(), out->getPtr());
	return out;
}

Cv::Image* resize_to_scale(Cv::Image* img, int width, int height)
{
	Cv::Image* out = 0;
	int width_old, height_old;
	float width_new, height_new;
	float wr, hr;

	width_old = img->getWidth();
	height_old = img->getHeight();

	wr = width_old / width;
	hr = height_old / height;

	//printf("Size request: %dx%d\n", width, height);
	//printf("Image size: %dx%d\n", width_old, height_old);

	//printf("Ratios: %f, %f\n", wr, hr);

	if(wr > hr) {
		//printf("A\n");
		width_new = width;
		height_new = width_new / width_old * height_old;
	}
	else {
		//printf("B\n");
		height_new = height;
		width_new = height_new / height_old * width_old;
	}

	//printf("New size: %fx%f\n", width_new, height_new);

	out = new Cv::Image((int)width_new, (int)height_new);
	cvResize(img->getPtr(), out->getPtr());
	return out;
}

} // end namespace Img
