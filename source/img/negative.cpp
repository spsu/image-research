#include "negative.hpp"
#include "../cv/Image.hpp"
#include <cv.h>

namespace Img {

void negative(Cv::Image* img)
{
	int width = 0;
	int height = 0;

	width = img->getWidth();
	height = img->getHeight();

	RgbImage pix = RgbImage(img->getPtr());

	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			pix[i][j].r = abs(pix[i][j].r - 255);
			pix[i][j].g = abs(pix[i][j].g - 255);
			pix[i][j].b = abs(pix[i][j].b - 255);
		}
	}
}


Cv::Image* negativeCopy(Cv::Image* in)
{
	Cv::Image* out = 0;
	int width = 0;
	int height = 0;

	width = in->getWidth();
	height = in->getHeight();

	out = new Cv::Image(width, height, 3); // TODO: Assumes 3 channels

	RgbImage pixIn = RgbImage(in->getPtr());
	RgbImage pixOut = RgbImage(out->getPtr());

	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			pixOut[i][j].r = abs(pixIn[i][j].r - 255);
			pixOut[i][j].g = abs(pixIn[i][j].g - 255);
			pixOut[i][j].b = abs(pixIn[i][j].b - 255);
		}
	}

	return out;
}

} // end namespace Img
