#include "Histogram.hpp"
#include "Image.hpp"
#include <stdio.h>

/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 */

namespace Cv {

Histogram::Histogram(int hSize, int hDimensions):
	histogram(0),
	size(hSize),
	dimensions(hDimensions)
{
	histogram = cvCreateHist(dimensions, &size, CV_HIST_ARRAY, NULL, 1);
}

Histogram::~Histogram()
{
	if(histogram != 0) {
		cvReleaseHist(&histogram);
	}
}

void Histogram::clear()
{
	cvClearHist(histogram);
}

float Histogram::query(int x, int y, int z)
{
	switch(dimensions) {
		case 1:
			return cvQueryHistValue_1D(histogram, x);
		case 2:
			return cvQueryHistValue_2D(histogram, x, y);
		case 3:
			return cvQueryHistValue_3D(histogram, x, y, z);
		default:
			fprintf(stderr,
				"Histogram::query() doesn't support n-dimensions, n > 3\n");
			return 0.0f;
	}
}

void Histogram::calculate(Image* image, bool accumulate)
{
	IplImage* iplImg[] = { image->getPtr() };

	if(dimensions != 1) {
		fprintf(stderr,
			"Histogram::calculate() only supports 1 dimension currently\n");
		return;
	}

	// TODO: Support the mask parameter
	cvCalcHist(iplImg, histogram, (int)accumulate, NULL);
}

} // end namespace Cv
