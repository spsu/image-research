/**
 * Copyright (c) 2010 Brandon Thomas Suit
 * http://possibilistic.org | echelon@gmail.com
 * Code available for use under the LGPL 2.
 * 
 * Cv::Mat
 *   Wrapper for CvMat and part of the OpenCV Arr->Mat->Img OO Hierarchy.
 *    TODO: Not much matrix support yet...
 *    TODO: Sparse mats, etc.
 */

#include "Mat.hpp"
#include <stdio.h>

namespace Cv {

Mat::Mat():
	Arr(),
	mat(0),
	image(0)
{
	// Nothing
	isOwner = true;
}

Mat::Mat(int rows, int cols, int type):
	Arr(),
	mat(0),
	image(0)
{
	mat = cvCreateMat(rows, cols, type);
	printf("Mat created\n");
}

Mat::Mat(const Mat& m):
	Arr(),
	mat(0),
	image(0)
{
	mat = cvCloneMat(m.mat);
	printf("Mat cloned\n");
}

/* TODO
Mat::Mat(int rows, int cols, std::vector data)
{
	double a[] = { 1,  2,  3,  4,
		           5,  6,  7,  8,
		           9, 10, 11, 12 };

	CvMat Ma=cvMat(3, 4, CV_64FC1, a);
}*/

Mat::~Mat()
{
	if(mat != NULL && isOwner) {
		cvReleaseMat(&mat);
		mat = 0;
	}
}

int Mat::getWidth()
{
	if(image != NULL) {
		return image->width;
	}
	else if(mat != NULL) {
		return mat->width;
	}
	return 0;
}

int Mat::getHeight()
{
	if(image != NULL) {
		return image->height;
	}
	else if(mat != NULL) {
		return mat->height;
	}
	return 0;
}

CvSize Mat::getSize() 
{ 
	if(image != NULL) {
		return cvGetSize(image); 
	}
	return cvGetSize(mat);
}

} // end namespace Cv
