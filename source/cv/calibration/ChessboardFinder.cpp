/**
 * Copyright (c) 2010 Brandon Thomas Suit
 * http://possibilistic.org | echelon@gmail.com
 * Code available for use under the LGPL 2.
 * 
 * Cv::Calibration::ChessboardFinder
 *   An easy mechanism for finding and recording chessboard corners in an
 *   image.
 */

#include "ChessboardCorners.hpp"
#include "Image.hpp"

namespace Cv {
namespace Calibration {

ChessboardCorners::ChessboardCorners(CvSize sz):
	boardSize(sz),
	boardArea(sz.width*sz.height)
{
	// nothing yet
}

ChessboardCorners::ChessboardCorners(int width, int height):
	boardSize(cvSize(width, height)),
	boardArea(width*height)
{
	// nothing yet
}

ChessboardCorners::~ChessboardCorners()
{
	while(!allCorners.empty()) {
		delete allCorners.back();
		allCorners.pop_back();
	}
}

bool ChessboardCorners::findCorners(Cv::Image* img, int flags)
{
	int cornerTotal = 0;
	CvPoint2D32f* corners = 0;
	int f = 0;
	int count = 0;

	corners = new CvPoint2D32f[boardArea];

	f = cvFindChessboardCorners(
			img->getPtr(), boardSize, 
			corners, &count, 
			flags	
	);

	if(f == 0 || count < boardArea) {
		delete corners;
		return false;
	}

	IplImage* gray = 0;
	cvCvtColor(img->getPtr(), gray, CV_BGR2GRAY);

	// Subpixel accuracy must be interpolated
	cvFindCornerSubPix(
			gray, corners, count, 
			cvSize(11, 11), cvSize(-1, -1),
			cvTermCriteria(
				CV_TERMCRIT_ITER + CV_TERMCRIT_EPS,	
				30, 0.01
			)
	);

	allCorners.push_back(corners);

	cvReleaseImage(&gray);
	return true;
}

int ChessboardCorners::numFound()
{
	return allCorners.size();
}

CvMat* ChessboardCorners::getObjectPoints()
{
	if(objectPoints == NULL) {
		genObjectPoints();
	}
	return objectPoints;
}

CvMat* ChessboardCorners::getImagePoints()
{
	if(imagePoints == NULL) {
		genImagePoints();
	}
	return imagePoints;
}

CvMat* ChessboardCorners::getPointCounts()
{
	int found = 0;

	found = numFound();

	if(numPoints != NULL) {
		CvSize sz = cvGetSize(pointCounts);
		printf("Size of numpoints array: %d, %d\n", sz.width, sz.height);// TEMP
		if(sz.width == found) {
			printf("Returning mat, already generated...\n"); // TEMP
			return pointCounts;
		}
		cvReleaseMat(&pointCounts);
	}

	pointCounts = cvCreateMat(found, 1, CV_64F);

	for(int i = 0; i < found; i++) {
		CV_MAT_ELEM(*pointCounts, int, i, 0) = boardArea;
	}
}

void ChessboardCorners::genObjectPoints()
{
	int found = 0;
	int len = 0;

	if(objectPoints != NULL) {
		cvReleaseMat(&objectPoints);
	}

	found = numFound();
	len = found * boardArea;

	// TODO: This is all wrong...
	objectPoints = cvCreateMat(found, 3, CV_64F); // CV_32FC1 ?

	for(int i = 0; i < boardSize.width; i++) {
		for(int i = 0; i < boardSize.height; i++) {
			CV_MAT_ELEM(*objectPoints, float, i, j) = i;
		}
	}
}

void ChessboardCorners::genImagePoints()
{
	int found = 0;
	int len = 0;

	if(imagePoints != NULL) {
		cvReleaseMat(&imagePoints);
	}

	found = numFound();

	imagePoints = cvCreateMat(num*boardArea, 2, CV_64F); // CV_32FC1 ?

	// TODO

}

} // end namespace Calibration
} // end namespace Cv
