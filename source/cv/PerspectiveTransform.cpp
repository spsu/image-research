#include "PerspectiveTransform.hpp"
#include "Image.hpp"
#include <stdio.h>
#include <math.h>

namespace Cv {

PerspectiveTransform::PerspectiveTransform(int w, int h):
	width(w),
	height(h),
	xTranslation(0),
	yTranslation(0),
	mat(0),
	isMatStale(false),
	fov(0.1),
	isNoClipping(false)
{
	mat = cvCreateMat(3, 3, CV_32FC1);
	cvZero(mat);

	// Does a good job initializing, too.
	reset();
}

PerspectiveTransform::PerspectiveTransform(Size sz):
	width(sz.width),
	height(sz.height),
	xTranslation(0),
	yTranslation(0),
	mat(0),
	isMatStale(false),
	fov(0.1),
	isNoClipping(false)
{
	mat = cvCreateMat(3, 3, CV_32FC1);
	cvZero(mat);

	// Does a good job initializing, too.
	reset();
}

PerspectiveTransform::~PerspectiveTransform()
{
	cvReleaseMat(&mat);
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

	isMatStale = false;
}

void PerspectiveTransform::setSrc(int i, float x, float y)
{
	src[i].x = x;
	src[i].y = y;

	isMatStale = true;
}

void PerspectiveTransform::setDst(int i, float x, float y)
{
	dst[i].x = x;
	dst[i].y = y;

	isMatStale = true;
}

void PerspectiveTransform::setTranslation(float x, float y)
{
	xTranslation = x;
	yTranslation = y;

	isMatStale = true;
}

void PerspectiveTransform::unsetTranslation()
{
	xTranslation = 0.0f;
	yTranslation = 0.0f;

	isMatStale = true; 
}

Size PerspectiveTransform::getClippedSize()
{
	Size sz;
	int minWidth = 0;
	int minHeight = 0;
	int maxWidth = 1;
	int maxHeight = 1;

	if(isMatStale) {
		updateMat();
	}

	//maxWidth = dst[0].x;
	//maxHeight = dst[0].y;
	
	for(unsigned int i = 0; i < 4; i++) {
		printf("Size[%d] w: %d, h: %d\n", i, (int)dst[i].x, (int)dst[i].y);
		if(maxWidth < dst[i].x) {
			maxWidth = (int)ceil(dst[i].x);
		}
		if(maxHeight < dst[i].y) {
			maxHeight = (int)ceil(dst[i].y);
		}
		if(minWidth > dst[i].x) {
			minWidth = (int)floor(dst[i].x);
		}
		if(minHeight > dst[i].y) {
			minHeight = (int)floor(dst[i].y);
		}
	}

	sz.width = maxWidth;
	sz.height = maxHeight;

	printf("Max: w %d, h %d\n", maxWidth, maxHeight);
	printf("Min: w %d, h %d\n", minWidth, minHeight);

	if(minWidth < 0) {
		sz.width += abs(minWidth);
	}
	if(minHeight < 0) {
		sz.height += abs(minHeight);
	}

	return sz; // TODO: Return (-1, -1) on failure
}

void PerspectiveTransform::setRotationX(float deg) 
{
	float theta = 0.0f;
	float sin_t = 0.0f;
	float cos_t = 0.0f;

	theta = deg2rad(deg);
	sin_t = (float)sin(theta);
	cos_t = (float)cos(theta);

	// XXX: Hack: 0.1 "pseudo-FOV" is necessary so points don't converge
	dst[2].x = dst[2].x + (sin_t * (width - 1) * fov);
	dst[2].y = cos_t * dst[2].y;
	dst[3].x = dst[3].x - (sin_t * (width - 1) * fov);
	dst[3].y = cos_t * dst[3].y;

	isMatStale = true; 
}

void PerspectiveTransform::setRotationY(float deg)
{
	float theta = 0.0f;
	float sin_t = 0.0f;
	float cos_t = 0.0f;

	theta = deg2rad(deg);
	sin_t = (float)sin(theta);
	cos_t = (float)cos(theta);

	// XXX: Hack: 0.1 "pseudo-FOV" is necessary so points don't converge
	dst[1].x = cos_t * dst[1].x;
	dst[1].y = dst[1].y - sin_t * (height - 1) * fov;
	dst[3].x = cos_t * dst[3].x;
	dst[3].y = dst[3].y + sin_t * (height - 1) * fov;

	isMatStale = true;
}

void PerspectiveTransform::setRotationZ(float deg, int x, int y)
{
	double theta = 0.0f;
	float sin_t = 0.0f;
	float cos_t = 0.0f;
	CvPoint2D32f out[4];

	theta = deg2rad(deg);
	sin_t = (float)sin(theta);
	cos_t = (float)cos(theta);

	// Reference: 
	// http://everything2.com/title/rotation
	// http://homepages.inf.ed.ac.uk/rbf/HIPR2/rotate.htm

	if(x == 0 && y == 0) {
		out[0].x = cos_t * dst[0].x - sin_t * dst[0].y;
		out[0].y = cos_t * dst[0].y + sin_t * dst[0].x;
		out[1].x = cos_t * dst[1].x - sin_t * dst[1].y;
		out[1].y = cos_t * dst[1].y + sin_t * dst[1].x;
		out[2].x = cos_t * dst[2].x - sin_t * dst[2].y;
		out[2].y = cos_t * dst[2].y + sin_t * dst[2].x;
		out[3].x = cos_t * dst[3].x - sin_t * dst[3].y;
		out[3].y = cos_t * dst[3].y + sin_t * dst[3].x;
	}
	else {
		out[0].x = cos_t * (dst[0].x - x) - sin_t * (dst[0].y - y) + x;
		out[0].y = cos_t * (dst[0].y - y) + sin_t * (dst[0].x - x) + y;
		out[1].x = cos_t * (dst[1].x - x) - sin_t * (dst[1].y - y) + x;
		out[1].y = cos_t * (dst[1].y - y) + sin_t * (dst[1].x - x) + y;
		out[2].x = cos_t * (dst[2].x - x) - sin_t * (dst[2].y - y) + x;
		out[2].y = cos_t * (dst[2].y - y) + sin_t * (dst[2].x - x) + y;
		out[3].x = cos_t * (dst[3].x - x) - sin_t * (dst[3].y - y) + x;
		out[3].y = cos_t * (dst[3].y - y) + sin_t * (dst[3].x - x) + y;
	}

	for(unsigned int i = 0; i < 4; i++) {
		dst[i] = out[i];
	}

	isMatStale = true; 
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
	float maxNegX = 0; // To prevent clipping, catches smallest val < 0
	float maxNegY = 0;

	// Work only with temp values
	for(unsigned int i = 0; i < 4; i++) {
		out[i].x = dst[i].x;
		out[i].y = dst[i].y;
	}

	// Shift negative values back into frame.
	if(isNoClipping) {
		for(unsigned int i = 0; i < 4; i++) {
			if(out[i].x < maxNegX) {
				maxNegX = out[i].x;
			}
			if(out[i].y < maxNegY) {
				maxNegY = out[i].y;
			} 
		}

		printf("MaxNegX: %f, MaxNegY: %f\n", maxNegX, maxNegY);

		for(unsigned int i = 0; i < 4; i++) {
			out[i].x += abs(maxNegX);
			out[i].y += abs(maxNegY);
		}
	}

	// Add translation
	for(unsigned int i = 0; i < 4; i++) {
		out[i].x += xTranslation;
		out[i].y += yTranslation;
	}

	cvGetPerspectiveTransform(src, out, mat);

	isMatStale = false;
}

void PerspectiveTransform::warpPerspective(Image* src, Image* dst)
{
	if(isMatStale) {
		updateMat();
	}

	cvWarpPerspective(src->getPtr(), dst->getPtr(), mat);
}

} // end namespace Cv
