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
	boardArea(sz.width*sz.height),
	objectPoints(0),
	imagePoints(0),
	pointCounts(0),
	matriciesDirty(true),
	tempGray(0)
{
	// nothing yet
}

ChessboardCorners::ChessboardCorners(int width, int height):
	boardSize(cvSize(width, height)),
	boardArea(width*height),
	objectPoints(0),
	imagePoints(0),
	pointCounts(0),
	matriciesDirty(true),
	tempGray(0)
{
	// nothing yet
}

ChessboardCorners::~ChessboardCorners()
{
	while(!allCorners.empty()) {
		delete allCorners.back();
		allCorners.pop_back();
	}

	// TODO: One if stmt

	if(objectPoints != NULL) {
		cvReleaseMat(&objectPoints);
	}
	if(imagePoints != NULL) {
		cvReleaseMat(&imagePoints);
	}
	if(pointCounts != NULL) {
		cvReleaseMat(&pointCounts);
	}
	if(tempGray != NULL) {
		cvReleaseImage(&tempGray);
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

	if(f == NULL || count < boardArea) {
		delete corners;
		return false;
	}

	// Only do once per instance!
	if(tempGray == NULL) {
		tempGray = cvCreateImage(img->getSize(), 8, 1); 
	}

	cvCvtColor(img->getPtr(), tempGray, CV_BGR2GRAY);

	// Subpixel accuracy must be interpolated
	cvFindCornerSubPix(
			tempGray, corners, count, 
			cvSize(11, 11), cvSize(-1, -1),
			cvTermCriteria(
				CV_TERMCRIT_ITER + CV_TERMCRIT_EPS,	
				30, 0.01
			)
	);

	allCorners.push_back(corners);

	matricesDirty = true;
	return true;
}

int ChessboardCorners::numFound()
{
	return allCorners.size();
}

CvMat* ChessboardCorners::getObjectPoints()
{
	if(matricesDirty) {
		generateMatrices();
	}
	return objectPoints;
}

CvMat* ChessboardCorners::getImagePoints()
{
	if(matricesDirty) {
		generateMatrices();
	}
	return imagePoints;
}

CvMat* ChessboardCorners::getPointCounts()
{
	if(matricesDirty) {
		generateMatrices();
	}
	return pointCounts;
}

void ChessboardCorners::generateMatrices()
{
	int found = 0;
	int len = 0;

	if(!matricesDirty) {
		return; // No need...
	}

	found = numFound();

	// =============
	// Object Points
	// =============

	if(objectPoints != NULL) {
		cvReleaseMat(&objectPoints);
	}

	len = found * boardArea;

	// TODO: This is all wrong...
	objectPoints = cvCreateMat(found, 3, CV_64F); // TODO: CV_32FC1 ?

	// Code adapted from [Bradski, Kaehler]
	for(int i = 0; i < allCorners.size(); i++) {
		for(int j = 0; j < boardArea) {
			int step = i * boardArea + j;
			CV_MAT_ELEM(*imagePoints, float, step, 0) = allCorners[i][j].x;
			CV_MAT_ELEM(*imagePoints, float, step, 1) = allCorners[i][j].y;
			CV_MAT_ELEM(*objectPoints, float, step, 0) = j/boardSize.width;
			CV_MAT_ELEM(*objectPoints, float, step, 1) = j%boardSize.width;
			CV_MAT_ELEM(*objectPoints, float, step, 2) = 0.0f;
		}
	}

	//for(int i = 0; i < boardSize.width; i++) {
	//	for(int i = 0; i < boardSize.height; i++) {
	//		CV_MAT_ELEM(*objectPoints, float, i, j) = i;
	//	}
	//}


	// ============
	// Image Points
	// ============

	if(imagePoints != NULL) {
		cvReleaseMat(&imagePoints);
	}

	imagePoints = cvCreateMat(num*boardArea, 2, CV_64F); // TODO: CV_32FC1 ?

	// TODO... how??


	// ============
	// Point Counts
	// ============

	if(numPoints != NULL) {
		cvReleaseMat(&pointCounts);
	}

	pointCounts = cvCreateMat(found, 1, CV_64F);

	for(int i = 0; i < found; i++) {
		CV_MAT_ELEM(*pointCounts, int, i, 0) = boardArea;
	}

	matricesDirty = false;
}

} // end namespace Calibration
} // end namespace Cv
