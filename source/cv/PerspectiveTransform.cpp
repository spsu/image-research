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
	isMatStale(false)
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

	// XXX: Hack: 0.3 "pseudo-FOV" is necessary so points don't converge
	dst[1].x = cos_t * dst[1].x;
	dst[1].y = dst[1].y - sin_t * (height - 1) * 0.1;
	dst[3].x = cos_t * dst[3].x;
	dst[3].y = dst[3].y + sin_t * (height - 1) * 0.1;

	//printf("==== POINTS AFTER Y ====\n");
	//printf("(%f, %f)\t\t(%f, %f)\n\n", dst[0].x, dst[0].y, dst[1].x, dst[1].y);
	//printf("(%f, %f)\t\t(%f, %f)\n\n\n\n", dst[2].x, dst[2].y, dst[3].x, dst[3].y);

	printf("==== NORMAL CALCULATION ====\n");
	updateMat();
	printMat();

	printf("==== LINEAR ALGEBRA CALCULATION ====\n");
	//reset();

	/* ORIGINAL WIKIPEDIA VALUES
	CV_MAT_ELEM(*mat, float, 0, 0) = cos_t;
	CV_MAT_ELEM(*mat, float, 0, 1) = 0.0f;
	CV_MAT_ELEM(*mat, float, 0, 2) = sin_t;
	CV_MAT_ELEM(*mat, float, 1, 0) = 0.0f;
	CV_MAT_ELEM(*mat, float, 1, 1) = 1.0f;
	CV_MAT_ELEM(*mat, float, 1, 2) = 0.0f;
	CV_MAT_ELEM(*mat, float, 2, 0) = sin_t * -1.0f;
	CV_MAT_ELEM(*mat, float, 2, 1) = 0.0f;
	CV_MAT_ELEM(*mat, float, 2, 2) = cos_t; */


	/*CV_MAT_ELEM(*mat, float, 0, 0) = cos_t;	// Main rotation point
	CV_MAT_ELEM(*mat, float, 0, 1) = 0.0f;
	CV_MAT_ELEM(*mat, float, 0, 2) = 0.0f;
	CV_MAT_ELEM(*mat, float, 1, 0) = 0.0f;
	CV_MAT_ELEM(*mat, float, 1, 1) = 1.0f;
	CV_MAT_ELEM(*mat, float, 1, 2) = 0.0f;
	CV_MAT_ELEM(*mat, float, 2, 0) = sin_t * -1.0f * 0.01f;
	CV_MAT_ELEM(*mat, float, 2, 1) = 0.0f;
	CV_MAT_ELEM(*mat, float, 2, 2) = 1.0f;

	updateMat2();

	printMat();*/

	isMatStale = true;
	//isMatStale = false;
}

void PerspectiveTransform::updateMat2()
{
	// Add translation. 
	CV_MAT_ELEM(*mat, float, 0, 2) += xTranslation;
	CV_MAT_ELEM(*mat, float, 1, 2) += yTranslation;
}

void PerspectiveTransform::setRotationZ(float deg)
{
	double theta = 0.0f;
	float sin_t = 0.0f;
	float cos_t = 0.0f;

	theta = deg2rad(deg);
	sin_t = (float)sin(theta);
	cos_t = (float)cos(theta);

	/*CV_MAT_ELEM(*mat, float, 0, 0) = cos_t;
	CV_MAT_ELEM(*mat, float, 0, 1) = sin_t * -1.0f;
	CV_MAT_ELEM(*mat, float, 0, 2) = 0.0f;
	CV_MAT_ELEM(*mat, float, 1, 0) = sin_t;
	CV_MAT_ELEM(*mat, float, 1, 1) = cos_t;
	CV_MAT_ELEM(*mat, float, 1, 2) = 0.0f;
	CV_MAT_ELEM(*mat, float, 2, 0) = 0.0f;
	CV_MAT_ELEM(*mat, float, 2, 1) = 0.0f;
	CV_MAT_ELEM(*mat, float, 2, 2) = 1.0f;*/


	printf("==== POINTS BEFORE Z ====\n");
	printf("(%f, %f)\t\t(%f, %f)\n\n", dst[0].x, dst[0].y, dst[1].x, dst[1].y);
	printf("(%f, %f)\t\t(%f, %f)\n\n\n\n", dst[2].x, dst[2].y, dst[3].x, dst[3].y);


	//		0				pi/2			pi				3pi/2			2pi
	//	 	0		45		90		135		180		225		270		315		360
	//	sin	0		0.707	1		0.707	0		-0.707	-1		-0.707	0
	//	cos	1		0.707	0		-0.707	-1		-0.707	0		0.707	1

	//	y0	0		-50		0		50		100		150		100		50		0	XXX XXX
	//	y1	0		50		100		150		100		50		0		-50		0
	//  y2	100		150		100		50		0		-50		0		50		100
	//  y3	100		50		0		-50		0		50		100		150		100

	//				45(-50)							225(150)

	// Amplitude: -50 to 150, -0.5 height to 1.5 height, A = 2

	//						PHASE					AMPLITUDE			
	/*dst[0].x = dst[0].x;
	dst[0].y = dst[0].y  + sin(theta + ((5*PI)/4) ) * (width-1) + 70;
	dst[1].x = dst[1].x;
	dst[1].y = dst[1].y  + sin(theta + ((7*PI)/4) ) * (width-1) + 70;

	dst[2].x = dst[2].x;
	dst[2].y = dst[2].y  + sin(theta + ((1*PI)/4) ) * (width-1);
	dst[3].x = dst[3].x;
	dst[3].y = dst[3].y  + sin(theta + ((3*PI)/4) ) * (width-1);*/

	int cx = 100;
	int cy = 100;

	/* dst[0].x = cos_t * (dst[0].x - cx) - sin_t * (dst[0].y - cy) + cx;
	dst[0].y = sin_t * (dst[0].x - cx) - cos_t * (dst[0].y - cy) + cy;
	dst[1].x = cos_t * (dst[1].x - cx) - sin_t * (dst[1].y - cy) + cx;
	dst[1].y = sin_t * (dst[1].x - cx) - cos_t * (dst[1].y - cy) + cy;
	dst[2].x = cos_t * (dst[2].x - cx) - sin_t * (dst[2].y - cy) + cx;
	dst[2].y = sin_t * (dst[2].x - cx) - cos_t * (dst[2].y - cy) + cy;
	dst[3].x = cos_t * (dst[3].x - cx) - sin_t * (dst[3].y - cy) + cx;
	dst[3].y = sin_t * (dst[3].x - cx) - cos_t * (dst[3].y - cy) + cy;*/

	// http://everything2.com/title/rotation
	dst[0].x = cos_t * dst[0].x - sin_t * dst[0].y;
	dst[0].y = cos_t * dst[0].y + sin_t * dst[0].x;
	dst[1].x = cos_t * dst[1].x - sin_t * dst[1].y;
	dst[1].y = cos_t * dst[1].y + sin_t * dst[1].x;
	dst[2].x = cos_t * dst[2].x - sin_t * dst[2].y;
	dst[2].y = cos_t * dst[2].y + sin_t * dst[2].x;
	dst[3].x = cos_t * dst[3].x - sin_t * dst[3].y;
	dst[3].y = cos_t * dst[3].y + sin_t * dst[3].x;

	printf("==== POINTS AFTER Z ====\n");
	printf("sin: %f\n", sin_t);
	printf("cos: %f\n", cos_t);
	printf("(%f, %f)\t\t(%f, %f)\n\n", dst[0].x, dst[0].y, dst[1].x, dst[1].y);
	printf("(%f, %f)\t\t(%f, %f)\n\n\n\n", dst[2].x, dst[2].y, dst[3].x, dst[3].y);

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
