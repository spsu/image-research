#include "Rectification.hpp"
#include "Calibration.hpp"
#include "../calibration/CamIntrinsics.hpp"
#include <stdio.h>

namespace Cv {
namespace Stereo {

Rectification::Rectification()
{
	rotationLeft    = cvCreateMat(3, 3, CV_64F);
	rotationRight   = cvCreateMat(3, 3, CV_64F);
	projectionLeft  = cvCreateMat(3, 4, CV_64F);
	projectionRight = cvCreateMat(3, 4, CV_64F);
	reprojection    = cvCreateMat(4, 4, CV_64F);
}

Rectification::~Rectification()
{
	cvReleaseMat(&rotationLeft);
	cvReleaseMat(&rotationRight);
	cvReleaseMat(&projectionLeft);
	cvReleaseMat(&projectionRight);
	cvReleaseMat(&reprojection);
}

bool Rectification::rectify(Cv::Calibration::CamIntrinsics* cam1,
							Cv::Calibration::CamIntrinsics* cam2, 
							Stereo::Calibration* calib, 
							int flags)
{
	/* TODO
	if(!calib1->isCalibrated() or !calib2->isCalibrated()) {
		fprintf(stderr, "Cannot rectify with an uncalibrated camera.\n")
		return false;
	}*/

	// TODO: cf. cvStereoRectifyUncalibrated()

	cvStereoRectify(
		// Input
		cam1->intrinsic, 
		cam2->intrinsic, 
		cam1->distortion, 
		cam2->distortion,
		cam1->imageSize,
		calib->rotation,
		calib->translation,

		// Output
		rotationLeft, 
		rotationRight,
		projectionLeft, 
		projectionRight,
		reprojection, // TODO: optional

		flags
	);

	return true; // TODO TEMP
}

/*TODO
bool Rectification::initUndistortMap()
{
	cvInitUndistortRectifyMap(
		// read docs
	);
}*/

} // end namespace Stereo
} // end namespace Cv
