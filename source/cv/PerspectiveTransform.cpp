#include "PerspectiveTransform.hpp"
#include <stdio.h>

namespace Cv {

//double PerspectiveTransform::PI = 3.14159265358979323846;

PerspectiveTransform::PerspectiveTransform(int w, int h):
	width(w),
	height(h),
	xTranslation(0),
	yTranslation(0),
	mat(0)
{
	mat = cvCreateMat(3, 3, CV_32FC1);
	cvZero(mat);

	// Does a good job initializing, too.
	reset();
}

PerspectiveTransform::~PerspectiveTransform()
{
	
}

void PerspectiveTransform::reset()
{
	src[0].x = 0;
	src[0].y = 0;
	src[1].x = width - 1;
	src[1].y = 0;
	src[2].x = 0;
	src[2].y = height - 1;
	src[3].x = width - 1;
	src[3].y = height - 1;

	dst[0].x = 0;
	dst[0].y = 0;
	dst[1].x = width - 1;
	dst[1].y = 0;
	dst[2].x = 0;
	dst[2].y = height - 1;
	dst[3].x = width - 1;
	dst[3].y = height - 1;
}

void PerspectiveTransform::setSrc(int i, float x, float y)
{
	src[i].x = x;
	src[i].y = y;
}

void PerspectiveTransform::setDst(int i, float x, float y)
{
	dst[i].x = x;
	dst[i].y = y;
}

void PerspectiveTransform::setTranslation(float x, float y)
{
	xTranslation = x;
	yTranslation = y;
}

void PerspectiveTransform::unsetTranslation()
{
	xTranslation = 0.0f;
	yTranslation = 0.0f;
}

void PerspectiveTransform::setRotationX(float deg) 
{
	float theta = 0.0f;
	float sin_t = 0.0f;
	float cos_t = 0.0f;

	theta = deg2rad(deg);
	sin_t = (float)sin(theta);
	cos_t = (float)cos(theta);


	printf("==== POINTS BEFORE X ====\n");
	printf("(%f, %f)\t\t(%f, %f)\n\n", dst[0].x, dst[0].y, dst[1].x, dst[1].y);
	printf("(%f, %f)\t\t(%f, %f)\n\n\n\n", dst[2].x, dst[2].y, dst[3].x, dst[3].y);

	// XXX: Hack: 0.3 "pseudo-FOV" is necessary so points don't converge

	dst[2].x = dst[2].x + (sin_t * (width - 1) * 0.1);
	dst[2].y = cos_t * dst[2].y;
	dst[3].x = dst[3].x - (sin_t * (width - 1) * 0.1);
	dst[3].y = cos_t * dst[3].y;

	printf("==== POINTS AFTER X ====\n");
	printf("(%f, %f)\t\t(%f, %f)\n\n", dst[0].x, dst[0].y, dst[1].x, dst[1].y);
	printf("(%f, %f)\t\t(%f, %f)\n\n\n\n", dst[2].x, dst[2].y, dst[3].x, dst[3].y);
}

void PerspectiveTransform::setRotationY(float deg)
{
	float theta = 0.0f;
	float sin_t = 0.0f;
	float cos_t = 0.0f;

	theta = deg2rad(deg);
	sin_t = (float)sin(theta);
	cos_t = (float)cos(theta);

	// XXX: Hack: 0.3 "pseudo-FOV" is necessary so points don't converge
	dst[1].x = cos_t * dst[1].x; // replaces (width - 1)
	dst[1].y = dst[1].y - sin_t * (height - 1) * 0.1;
	dst[3].x = cos_t * dst[3].x;
	dst[3].y = dst[3].y + sin_t * (height - 1) * 0.1; // OLD: height - yp1;

	printf("==== POINTS AFTER Y ====\n");
	printf("(%f, %f)\t\t(%f, %f)\n\n", dst[0].x, dst[0].y, dst[1].x, dst[1].y);
	printf("(%f, %f)\t\t(%f, %f)\n\n\n\n", dst[2].x, dst[2].y, dst[3].x, dst[3].y);
}


void PerspectiveTransform::printMat()
{
	printf("[\n");
	for(unsigned int i = 0; i < 3; i++) {
		printf("   [");
		for(unsigned int j = 0; j < 3; j++) {
			float val = (float)CV_MAT_ELEM(*mat, float, i, j);
			printf("\t%f", val);
		}
		printf("  ]\n");
	}
	printf("]\n");
}

void PerspectiveTransform::updateMat()
{
	CvPoint2D32f out[4];

	// Work only with temp values
	for(unsigned int i = 0; i < 4; i++) {
		out[i].x = dst[i].x;
		out[i].y = dst[i].y;
	}

	// Shift negative values back into frame. // TODO: Interesting idea.
	/*float maxNegX = 0;
	float maxNegY = 0;
	for(unsigned int i = 0; i < 4; i++) {
		if(out[i].x < maxNegX) {
			maxNegX = out[i].x;
		}
		if(out[i].y < maxNegY) {
			maxNegY = out[i].y;
		} 
	}
	for(unsigned int i = 0; i < 4; i++) {	// TODO: May need to record that we did this
		out[i].x += abs(maxNegX);			// Since this shifts for each theta value, we'll
		out[i].y += abs(maxNegY);			// need these numbers to properly align images
	}*/

	// Add translation
	for(unsigned int i = 0; i < 4; i++) {
		out[i].x += xTranslation;
		out[i].y += yTranslation;
	}

	cvGetPerspectiveTransform(src, out, mat);
}

} // end namespace Cv
