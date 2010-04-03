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
	patternSize(sz)
{
	// nothing yet
}

ChessboardCorners::ChessboardCorners(int width, int height):
	patternSize(cvSize(width, height))
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

	cornerTotal = patternSize.width * patternSize.height;

	corners = new CvPoint2D32f[cornerTotal];

	f = cvFindChessboardCorners(
			img->getPtr(), patternSize, 
			corners, &count, 
			flags	
	);

	if(f == 0 || count < cornerTotal) {
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

} // end namespace Calibration
} // end namespace Cv
