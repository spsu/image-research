#include "Rectification.hpp"
#include "Calibration.hpp"
#include <stdio.h>

namespace Cv {
namespace Stereo {

Rectification::Rectification():
	rotationLeft(0), rotationRight(0),
	projectionLeft(0), projectionRight(0),
	reprojection(0)
{
	rotationLeft  =  cvCreateMat(3, 3, CV_64F);
	rotationRight =  cvCreateMat(3, 3, CV_64F);

	projectionLeft  = cvCreateMat(3, 4, CV_64F);
	projectionRight = cvCreateMat(3, 4, CV_64F);

	reprojection = cvCreateMat(4, 4, CV_64F);
}

Rectification::~Rectification()
{
	cvReleaseMat(&rotationLeft);
	cvReleaseMat(&rotationRight);
	cvReleaseMat(&projectionLeft);
	cvReleaseMat(&projectionRight);
	cvReleaseMat(&reprojection);
}

bool Rectification::Rectify(Calibration* calib1, Calibration* calib2, int flags)
{
	/* TODO
	if(!calib1->isCalibrated() or !calib2->isCalibrated()) {
		fprintf(stderr, "Cannot rectify with an uncalibrated camera.\n")
		return false;
	}*/

	cvStereoRectify(
		// Input
		camMatrix1, camMatrix2, distCoeff1, distCoeff2,
		cvSize::imgSize,
		rotation, translation
		// Output
		rotationLeft, rotationRight,
		projectionLeft, projectionRight,
		reprojection, // TODO: optional
		flags
	);
	// TODO: cf. cvStereoRectifyUncalibrated()
}

/*bool Rectification::initUndistortMap()
{
	// TODO
	cvInitUndistortRectifyMap(
		// read docs
	);
}*/

} // end namespace Stereo
} // end namespace Cv
