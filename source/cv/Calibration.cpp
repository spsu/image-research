#include "Calibration.hpp"
#include "Image.hpp"
#include <stdio.h>

namespace Cv {

Calibration::Calibration(int boardW, int boardH, int num):
	intrinsics(0),
	distortion(0),
	xMap(0),
	yMap(0),
	size(cvSize(boardW, boardH)),
	numToFind(num),
	numFound(0),
	calibrated(false),
	iterData(0)
{
	// Nothing
}

Calibration::~Calibration()
{
	if(intrinsics != NULL) {
		cvReleaseMat(&intrinsics);
		cvReleaseMat(&distortion);
	}
	if(xMap != NULL) {
		cvReleaseImage(&xMap);
		cvReleaseImage(&yMap);
	}
	if(iterData) {
		delete iterData; // TODO: BoardIterData DTOR in hpp file. 
	}
}

void Calibration::setBoardParams(int boardW, int boardH, int num)
{
	// Note: We don't want to change these params while finding boards.
	size = cvSize(boardW, boardH);
	numToFind = num;
}

// This code based on O'Reilly's text on OpenCV,
// "Learning OpenCV" [Bradski & Kaehler 2008]
// Chapter 11: Camera Models and Calibration, pg 398 - 401
bool Calibration::findBoardIter(Image* im)
{
	IplImage* img = 0;
	/*IplImage* gray = 0;
	CvMat* imgPts = 0;
	CvMat* objPts = 0;
	CvMat* ptCounts = 0;
	CvPoint2D32f* corners;
	int area = 0;*/

	if(calibrated || numToFind <= numFound) {
		fprintf(stderr, "Calibration::findBoardIter() already found enough.\n");
		return false;
	}

	if(im == NULL) {
		fprintf(stderr, "Calibration::findBoardIter() no image given.\n");
		return false;
	}

	img = im->getPtr();

	// Initialize the iteration data that we will reuse on every call of this 
	// method. Initializing once and saving makes things faster. 
	if(iterData == NULL) {
		iterData = new BoardIterData();
		iterData->area = size.height * size.width; 

		iterData->imgPts = cvCreateMat(numToFind * iterData->area, 2, CV_32FC1);
		iterData->objPts = cvCreateMat(numToFind * iterData->area, 3, CV_32FC1);
		iterData->ptCounts = cvCreateMat(numToFind, 1, CV_32FC1);

		iterData->corners = new CvPoint2D32f[numToFind];
		iterData->gray = cvCreateImage(cvGetSize(img), 8, 1); // for subpixel
	}

	// Find chessboard 
	iterData->found = cvFindChessboardCorners(
				img, 
				size, 
				iterData->corners,
				&iterData->cornerCnt, 
				CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS
	);

	if(!iterData->found) {
		return false;
	}
	printf("Stage 1\n");


	IplImage* gray = cvCreateImage(cvGetSize(img), 8, 1); // for subpixel

	// Get subpixel accuracy
	//cvCvtColor(img, iterData->gray, CV_BGR2GRAY);
	cvCvtColor(img, gray, CV_BGR2GRAY);
	cvFindCornerSubPix(gray, iterData->corners, iterData->cornerCnt, 
					 cvSize(11,11), cvSize(-1,-1),
					 cvTermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1)
	);

	if(iterData->cornerCnt != iterData->area) {
		return false;
	}

	// Save the board...
	iterData->step = numFound * iterData->area;


	for(int i = iterData->step, j = 0; j < iterData->area; i++, j++) {
		CV_MAT_ELEM(*iterData->imgPts, float, i, 0) = iterData->corners[j].x;
		CV_MAT_ELEM(*iterData->imgPts, float, i, 1) = iterData->corners[j].y;
		CV_MAT_ELEM(*iterData->objPts, float, i, 0) = j / size.width; 
		CV_MAT_ELEM(*iterData->objPts, float, i, 1) = j % size.width; 
		CV_MAT_ELEM(*iterData->objPts, float, i, 2) = 0.0f; 
	}
	CV_MAT_ELEM(*iterData->ptCounts, int, numFound, 0) = iterData->area;

	numFound += 1;
	if(numFound == numToFind) {
		calibrated = true;
	}
	return true;
}

void Calibration::drawBoardIter(Image* im)
{
	cvDrawChessboardCorners(im->getPtr(), size, iterData->corners, 
							iterData->cornerCnt, iterData->found);
}

bool Calibration::findAndDrawBoardIter(Image* im)
{
	if(!findBoardIter(im)) {
		return false;
	}
	drawBoardIter(im);
	return true;
}



} // end namespace Cv
