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

bool Calibration::loadIntrinsics(std::string filename)
{
	CvMat* data = 0;

	if(isCalibrated()) {
		fprintf(stderr, 
			"Cannot load intrinsics for an already-calibrated camera.\n");
		return false;
	}

	data = (CvMat*)cvLoad(filename.c_str());
	if(data == NULL) {
		fprintf(stderr, "Could not load data from file.\n");
		return false;
	}

	intrinsics = data;
	return true;
}

bool Calibration::loadDistortion(std::string filename)
{
	CvMat* data = 0;

	if(isCalibrated()) {
		fprintf(stderr, 
			"Cannot load distortion for an already-calibrated camera.\n");
		return false;
	}

	data = (CvMat*)cvLoad(filename.c_str());
	if(data == NULL) {
		fprintf(stderr, "Could not load data from file.\n");
		return false;
	}

	distortion = data;
	return true;
}

bool Calibration::doGenerateMap(Cv::Image* img)
{
	if(isCalibrated()) {
		return false;
	}
	if(intrinsics == NULL || distortion == NULL) {
		return false;
	}

	generateMap(img->getPtr());

	if(xMap == NULL || yMap == NULL) {
		fprintf(stderr, 
			"Calibration::doGenerateMap() could not generate x or y maps\n");
		return false;
	}

	calibrated = true;
	return true;
}

bool Calibration::saveIntrinsics(std::string filename)
{
	if(!isCalibrated()) {
		return false;
	}
	cvSave(filename.c_str(), intrinsics);
	return true;
}

bool Calibration::saveDistortion(std::string filename)
{
	if(!isCalibrated()) {
		return false;
	}
	cvSave(filename.c_str(), distortion);
	return true;
}

// This code based on O'Reilly's text on OpenCV,
// "Learning OpenCV" [Bradski & Kaehler 2008]
// Chapter 11: Camera Models and Calibration, pg 398 - 401
bool Calibration::findBoardIter(Image* im)
{
	IplImage* img = 0;

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
		CV_MAT_ELEM(*iterData->objPts, float, i, 0) = j / size.width; 
		CV_MAT_ELEM(*iterData->objPts, float, i, 1) = j % size.width; 
		CV_MAT_ELEM(*iterData->objPts, float, i, 2) = 0.0f; 
	}
	CV_MAT_ELEM(*iterData->ptCounts, int, numFound, 0) = iterData->area;

	numFound += 1;

	// Finalize calibration... 
	if(numFound == numToFind) {
		generateIntrinsics(img); // sets calibrated = true
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

int Calibration::getNumFound()
{
	return numFound;
}

bool Calibration::undistort(Image* im)
{
	IplImage* clone = 0;

	if(!calibrated) {
		fprintf(stderr, "Calibration::undistort() can't use if uncalibrated\n");
		return false;
	}
	if(xMap == NULL || yMap == NULL) {
		fprintf(stderr, "Calibration::undistort() x and/or y maps are NULL\n");
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

void Calibration::generateIntrinsics(IplImage* img)
{
	if(calibrated || numFound < numToFind) {
		return;
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
		cvGetSize(img), 
		intrinsics,
		distortion,
		NULL,
		NULL,
		0
	);

	generateMap(img);

	calibrated = true;
}

void Calibration::generateMap(IplImage* img)
{
	if(intrinsics == NULL || distortion == NULL) {
		fprintf(stderr,
			"Calibration::generateMap() intrinsics or distortion NULL\n");
		return;
	}
	printf("Generating map...\n");

	xMap = cvCreateImage(cvGetSize(img), IPL_DEPTH_32F, 1);
	yMap = cvCreateImage(cvGetSize(img), IPL_DEPTH_32F, 1);

	cvInitUndistortMap(
		intrinsics,
		distortion,
		xMap,
		yMap
	);
}

} // end namespace Cv
