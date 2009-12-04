#include "grayscale.hpp"
#include "../cv/Image.hpp"
#include <cv.h>
#include <stdlib.h>

namespace Img {

void grayscale(Cv::Image* img)
{
	int width, height;
	int r, g, b;
	int avg;

	width = img->getWidth();
	height = img->getHeight();

	RgbPix pix = img->getPix(); // TODO: Rename Cv::Pix

	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			r = pix[i][j].r;
			g = pix[i][j].g;
			b = pix[i][j].b;

			avg = (r+g+b)/3;
			pix[i][j].r = avg;
			pix[i][j].g = avg;
			pix[i][j].b = avg;
		}
	}
}

Cv::Image* grayscaleCopy(Cv::Image* in)
{
	int width, height;
	int r, g, b;
	int avg;
	Cv::Image* out = 0;

	width = in->getWidth();
	height = in->getHeight();

	out = new Cv::Image(width, height, 1);

	RgbPix pixIn = in->getPix(); // TODO: Rename Cv::Pix
	RgbPix pixOut = out->getPix(); // TODO: Rename Cv::Pix

	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			r = pixIn[i][j].r;
			g = pixIn[i][j].g;
			b = pixIn[i][j].b;

			avg = (r+g+b)/3;
			pixOut[i][j].r = avg;
			pixOut[i][j].g = avg;
			pixOut[i][j].b = avg;
		}
	}

	return out;
}

} // end namespace Img

