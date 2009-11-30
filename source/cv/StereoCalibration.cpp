#include "StereoCalibration.hpp"
#include "Image.hpp"
#include <stdio.h>

namespace Cv {

StereoCalibration::StereoCalibration(int boardW, int boardH, int num):
	// State
	stereoState(STEREO_INSTANTIATED),
	// Matrices 
	intrinsics1(0),
	intrinsics2(0),
	distortion1(0),
	distortion2(0),
	rotation(0),
	translation(0),
	essential(0),
	fundamental(0),
	rotation1(0),
	rotation2(0),
	projection1(0),
	projection2(0),
	reprojection(0),
	// Calibration process
	boardSize(cvSize(boardW, boardH)),
	frameSize(cvSize(0,0)),
	numToFind(num),
	numFound(0),
	iterData(0)
{
	// Nothing
}

StereoCalibration::~StereoCalibration()
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

void StereoCalibration::setBoardParams(int boardW, int boardH, int num)
{
	// XXX Note: We don't want to change these params while finding boards.
	boardSize = cvSize(boardW, boardH);

	if(num < 8) {
		fprintf(stderr, 
		  "StereoCalibration::setBoardParams(): num < 10, so setting to 10.\n");
		numToFind = 10;
	}
	else {
		numToFind = num;
	}
}

void StereoCalibration::setCameraFrameSize(int width, int height)
{
	frameSize = cvSize(width, height);
}



// XXX NOTE: Returns true only if found in BOTH images. (XXX Not sure if this
// is the desired behavior XXX)
bool StereoCalibration::findBoards(Image* im1, Image* im2)
{
	IplImage* img1 = 0;
	IplImage* img2 = 0;

	if(numToFind <= numFound) {
		fprintf(stderr, 
			"StereoCalibration::findBoards() already found enough.\n");
		return false;
	}

	if(im1 == NULL || im2 == NULL) {
		fprintf(stderr, 
			"StereoCalibration::findBoards() one or both images NULL.\n");
		return false;
	}

	img1 = im1->getPtr();
	img2 = im2->getPtr();




}








bool StereoCalibration::doGenerateMap()
{
	// Error conditions
	if(isCalibrated()) {
		return false;
	}
	if(intrinsics == NULL || distortion == NULL) {
		return false;
	}
	if(frameSize.width == 0 || frameSize.height == 0) {
		return false;
	}

	if(!generateMap()) {
		fprintf(stderr, 
			"StereoCalibration::doGenerateMap() error generating undistort map.\n");
		return false;
	}

	calibrated = true;
	return true;
}


// This code based on O'Reilly's text on OpenCV,
// "Learning OpenCV" [Bradski & Kaehler 2008]
// Chapter 11: Camera Models and Calibration, pg 398 - 401
bool StereoCalibration::findBoardIter(Image* im)
{
	IplImage* img = 0;

	if(calibrated || numToFind <= numFound) {
		fprintf(stderr, "StereoCalibration::findBoardIter() already found enough.\n");
		return false;
	}

	if(im == NULL) {
		fprintf(stderr, "StereoCalibration::findBoardIter() no image given.\n");
		return false;
	}

	img = im->getPtr();

	// Initialize the iteration data that we will reuse on every call of this 
	// method. Initializing once and saving makes things faster. 
	if(iterData == NULL) {
		iterData = new BoardIterData();
		iterData->area = boardSize.height * boardSize.width; 

		iterData->imgPts = cvCreateMat(numToFind * iterData->area, 2, CV_32FC1);
		iterData->objPts = cvCreateMat(numToFind * iterData->area, 3, CV_32FC1);
		iterData->ptCounts = cvCreateMat(numToFind, 1, CV_32FC1);

		iterData->corners = new CvPoint2D32f[numToFind];
		iterData->gray = cvCreateImage(cvGetSize(img), 8, 1); // for subpixel
	}

	// Find chessboard 
	iterData->found = cvFindChessboardCorners(
				img, 
				boardSize, 
				iterData->corners,
				&iterData->cornerCnt, 
				CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS
	);

	if(!iterData->found) {
		return false;
	}


	// Get subpixel accuracy
	IplImage* gray = cvCreateImage(cvGetSize(img), 8, 1); // XXX: Note below
	//cvCvtColor(img, iterData->gray, CV_BGR2GRAY); // XXX: Why won't this work?
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
		CV_MAT_ELEM(*iterData->objPts, float, i, 0) = j / boardSize.width; 
		CV_MAT_ELEM(*iterData->objPts, float, i, 1) = j % boardSize.width; 
		CV_MAT_ELEM(*iterData->objPts, float, i, 2) = 0.0f; 
	}
	CV_MAT_ELEM(*iterData->ptCounts, int, numFound, 0) = iterData->area;

	numFound += 1;

	// Finalize calibration... 
	if(numFound == numToFind) {
		generateIntrinsics(); // sets calibrated = true
	}
	return true;
}

void StereoCalibration::drawBoardIter(Image* im)
{
	cvDrawChessboardCorners(im->getPtr(), boardSize, iterData->corners, 
							iterData->cornerCnt, iterData->found);
}

bool StereoCalibration::findAndDrawBoardIter(Image* im)
{
	if(!findBoardIter(im)) {
		return false;
	}
	drawBoardIter(im);
	return true;
}

int StereoCalibration::getNumFound()
{
	return numFound;
}

bool StereoCalibration::undistort(Image* im)
{
	IplImage* clone = 0;

	if(!calibrated) {
		fprintf(stderr, "StereoCalibration::undistort() can't use if uncalibrated\n");
		return false;
	}
	if(xMap == NULL || yMap == NULL) {
		fprintf(stderr, "StereoCalibration::undistort() x and/or y maps are NULL\n");
		return false;
	}

	clone = cvCloneImage(im->getPtr());
	cvRemap(clone, im->getPtr(), xMap, yMap);

	cvReleaseImage(&clone);

	return true;
}

/*Image* Calibration::getUndistorted(Image* im)
{
	IplImage* img2 = 0;

	

	return new Image(img2);
}*/

// ===================== PROTECTED METHODS ================================== //

bool StereoCalibration::generateIntrinsics()
{
	if(calibrated || numFound < numToFind) {
		return false;
	}
	printf("Generating intrinsics...\n");

	// Must resize matrices
	CvMat* objPts = cvCreateMat(numFound*iterData->area, 3, CV_32FC1);
	CvMat* imgPts = cvCreateMat(numFound*iterData->area, 2, CV_32FC1);
	CvMat* ptCounts = cvCreateMat(numFound, 1, CV_32SC1);

	for(int i = 0; i < numFound*iterData->area; i++) {
		CV_MAT_ELEM(*imgPts, float, i, 0) = 
			CV_MAT_ELEM(*iterData->imgPts, float, i, 0);

		CV_MAT_ELEM(*imgPts, float, i, 1) = 
			CV_MAT_ELEM(*iterData->imgPts, float, i, 1);
		
		CV_MAT_ELEM(*objPts, float, i, 0) = 
			CV_MAT_ELEM(*iterData->objPts, float, i, 0);

		CV_MAT_ELEM(*objPts, float, i, 1) = 
			CV_MAT_ELEM(*iterData->objPts, float, i, 1);

		CV_MAT_ELEM(*objPts, float, i, 2) = 
			CV_MAT_ELEM(*iterData->objPts, float, i, 2);
	}

	for(int i = 0; i < numFound; i++) {
		CV_MAT_ELEM(*ptCounts, int, i, 0) = 
			CV_MAT_ELEM(*iterData->ptCounts, int, i, 0);
	}

	intrinsics = cvCreateMat(3, 3, CV_32FC1);
	distortion = cvCreateMat(5, 1, CV_32FC1);

	CV_MAT_ELEM(*intrinsics, float, 0, 0) = 1.0f;
	CV_MAT_ELEM(*intrinsics, float, 1, 1) = 1.0f;

	cvCalibrateCamera2(
		objPts, 
		imgPts,
		ptCounts,
		frameSize, 
		intrinsics,
		distortion,
		NULL,
		NULL,
		0
	);

	if(!generateMap()) {
		return false;
	}

	calibrated = true;
	return true;
}

bool StereoCalibration::generateMap()
{
	if(intrinsics == NULL || distortion == NULL) {
		fprintf(stderr,
			"StereoCalibration::generateMap() intrinsics or distortion NULL\n");
		return false;
	}
	if(frameSize.width == 0 || frameSize.height == 0) {
		fprintf(stderr,
			"StereoCalibration::generateMap() frameSize has not been set!\n");
		return false;
	}

	xMap = cvCreateImage(frameSize, IPL_DEPTH_32F, 1);
	yMap = cvCreateImage(frameSize, IPL_DEPTH_32F, 1);

	cvInitUndistortMap(
		intrinsics,
		distortion,
		xMap,
		yMap
	);

	if(xMap == NULL || yMap == NULL) {
		fprintf(stderr, 
			"StereoCalibration::generateMap() could not generate x or y maps\n");
		return false;
	}

	return true; // XXX: assumed. 
}


// ====================================== PRIVATE METHODS =================== //

void StereoCalibration::setupMatrices()
{
	// Camera Intrinsics & Calibration 
	intrinsics1 = cvCreateMat(3, 3, CV_64F);
	intrinsics2 = cvCreateMat(3, 3, CV_64F);
	distortion1 = cvCreateMat(1, 5, CV_64F);
	distortion2 = cvCreateMat(1, 5, CV_64F);
	rotation    = cvCreateMat(3, 3, CV_64F);
	translation = cvCreateMat(3, 1, CV_64F);
	essential   = cvCreateMat(3, 3, CV_64F);
	fundamental = cvCreateMat(3, 3, CV_64F);

	// Stereo Rectification
	rotation1	= cvCreateMat(3, 3, CV_64F);
	rotation2	= cvCreateMat(3, 3, CV_64F);
	projection1 = cvCreateMat(3, 4, CV_64F);
	projection2 = cvCreateMat(3, 4, CV_64F);
	reprojection = cvCreateMat(4, 4, CV_64F);
}




} // end namespace Cv
