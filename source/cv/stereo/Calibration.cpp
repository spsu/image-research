#include "Calibration.hpp"

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

bool Calibration::calibrate(int flags)
{
	cvStereoCalibrate(
		// In
		&objPts,
		&imgPts1, &imgPts2,
		&npts,

		// In/Inout/Out
		cam1->intrinsics,
		cam1->distortion,
		cam2->intrinsics,
		cam2->distortion,

		// Out
		rotation,
		translation,
		essential,		// TODO: Optional
		fundamental		// TODO: Optional
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

}


} // end namespace Stereo
} // end namespace Cv
