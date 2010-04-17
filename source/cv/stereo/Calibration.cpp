#include "Calibration.hpp"
#include "../calibration/CamIntrinsics.hpp"
#include "../calibration/ChessboardFinder.hpp"
#include <stdio.h>

namespace Cv {
namespace Stereo {

Calibration::Calibration():
	rotation(0), 
	translation(0),
	essential(0), 
	fundamental(0)
{
	rotation = cvCreateMat(3, 3, CV_64F);
	translation = cvCreateMat(3, 1, CV_64F);
	essential = cvCreateMat(3, 3, CV_64F);
	fundamental = cvCreateMat(3, 3, CV_64F);
}

Calibration::~Calibration()
{
	cvReleaseMat(&rotation);
	cvReleaseMat(&translation);
	cvReleaseMat(&essential);
	cvReleaseMat(&fundamental);
}

bool Calibration::calibrate(Cv::Calibration::ChessboardFinder* finder1,
							Cv::Calibration::ChessboardFinder* finder2,
							Cv::Calibration::CamIntrinsics* cam1,
							Cv::Calibration::CamIntrinsics* cam2, 
							int flags)
{
	finder1->generateMatrices();
	finder2->generateMatrices();

	cvStereoCalibrate(
		// In
		finder1->objectPoints, // XXX Not &objectPoints !
		finder1->imagePoints, 
		finder2->imagePoints,
		finder1->pointCounts,

		// In/Inout/Out (Depends upon flag!)
		cam1->intrinsic,
		cam1->distortion,
		cam2->intrinsic,
		cam2->distortion,

		cam1->imageSize,

		// Out
		rotation,
		translation,
		essential,		// TODO: Optional
		fundamental,	// TODO: Optional
		cvTermCriteria(
			CV_TERMCRIT_ITER + CV_TERMCRIT_EPS,
			100,
			1e-5
		),
		flags
	);

	/* Flags:
		CV_CALIB_FIX_INTRINSIC
		CV_CALIB_USE_INTRINSIC_GUESS */


	return true; // TODO: TEMP
}


} // end namespace Stereo
} // end namespace Cv
