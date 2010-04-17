/**
 * Copyright (c) 2010 Brandon Thomas Suit
 * http://possibilistic.org | echelon@gmail.com
 * Code available for use under the LGPL 2.
 * 
 * Cv::Calibration::ChessboardFinder
 *   An easy mechanism for finding and recording chessboard corners in an
 *   image.
 */

#include "ChessboardFinder.hpp"
#include "ChessboardCorners.hpp"
#include "../Image.hpp"

namespace Cv {
namespace Calibration {

ChessboardFinder::ChessboardFinder(CvSize sz):
	boardSize(sz),
	boardArea(sz.width*sz.height),
	objectPoints(0),
	imagePoints(0),
	pointCounts(0),
	matricesDirty(true),
	tempGray(0)
{
	// nothing yet
}

ChessboardFinder::ChessboardFinder(int width, int height):
	boardSize(cvSize(width, height)),
	boardArea(width*height),
	objectPoints(0),
	imagePoints(0),
	pointCounts(0),
	matricesDirty(true),
	tempGray(0)
{
	// nothing yet
}

ChessboardFinder::~ChessboardFinder()
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

bool ChessboardFinder::findCorners(Cv::Image* img, int flags)
{
	//int cornerTotal = 0;
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

ChessboardCorners* ChessboardFinder::getCorners(Cv::Image* img, int flags)
{
	//int cornerTotal = 0;
	ChessboardCorners* corners = 0;
	int f = 0;
	int count = 0;

	corners = new ChessboardCorners(boardSize);

	f = cvFindChessboardCorners(
			img->getPtr(), boardSize, 
			corners->corners, &corners->numFound, 
			flags	
	);

	if(f == 0 || count < boardArea) {
		//delete corners;
		//return false;
		return corners;
	}

	// Only do once per instance!
	if(tempGray == NULL) {
		tempGray = cvCreateImage(img->getSize(), 8, 1); 
	}

	cvCvtColor(img->getPtr(), tempGray, CV_BGR2GRAY);

	// Subpixel accuracy must be interpolated
	cvFindCornerSubPix(
			tempGray, corners->corners, corners->numFound, 
			cvSize(11, 11), cvSize(-1, -1),
			cvTermCriteria(
				CV_TERMCRIT_ITER + CV_TERMCRIT_EPS,	
				30, 0.01
			)
	);

	//allCorners.push_back(corners);
	//matricesDirty = true;

	return corners;
}

int ChessboardFinder::numFound()
{
	return allCorners.size();
}

CvMat* ChessboardFinder::getObjectPoints()
{
	if(matricesDirty) {
		generateMatrices();
	}
	return objectPoints;
}

CvMat* ChessboardFinder::getImagePoints()
{
	if(matricesDirty) {
		generateMatrices();
	}
	return imagePoints;
}

CvMat* ChessboardFinder::getPointCounts()
{
	if(matricesDirty) {
		generateMatrices();
	}
	return pointCounts;
}

void ChessboardFinder::generateMatrices()
{
	int found = 0;
	int len = 0;

	if(!matricesDirty) {
		return; // No need...
	}

	found = numFound();

	if(found == 0) {
		fprintf(stderr, "No chessboards found, cannot generate matrices.\n");
		return;
	}

	// =============
	// Object Points
	// =============

	if(objectPoints != NULL) {
		cvReleaseMat(&objectPoints);
	}

	len = found * boardArea;

	// TODO: This is all wrong...
	objectPoints = cvCreateMat(found, 3, CV_64F); // TODO: CV_32FC1 ?

	if(imagePoints != NULL) {
		cvReleaseMat(&imagePoints);
	}

	imagePoints = cvCreateMat(found*boardArea, 2, CV_64F); // TODO: CV_32FC1 ?


	// Code adapted from [Bradski, Kaehler]
	for(unsigned int i = 0; i < allCorners.size(); i++) {
		for(int j = 0; j < boardArea; j++) {
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


	// TODO... how??


	// ============
	// Point Counts
	// ============

	if(pointCounts != NULL) {
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
