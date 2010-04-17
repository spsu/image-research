#include "CamIntrinsics.hpp"
#include "ChessboardFinder.hpp"
#include <stdio.h>

namespace Cv {
namespace Calibration {

CamIntrinsics::CamIntrinsics(int imWidth, int imHeight):
	imageSize(cvSize(imWidth, imHeight)),
	intrinsic(0),
	distortion(0),
	rotation(0),
	translation(0)
{
	// Nothing yet
}

CamIntrinsics::CamIntrinsics(CvSize imgSize):
	imageSize(imgSize),
	intrinsic(0),
	distortion(0),
	rotation(0),
	translation(0)
{
	// Nothing yet
}

CamIntrinsics::~CamIntrinsics()
{
	if(intrinsic != NULL) {
		cvReleaseMat(&intrinsic);
		cvReleaseMat(&distortion);
	}
	
	if(rotation != NULL) {
		cvReleaseMat(&rotation);
		cvReleaseMat(&translation);
	}
}

bool CamIntrinsics::calibrateCam(ChessboardFinder* finder)
{
	//int numFound = 0;
	int flags = 0;	

	if(finder->numFound() == 0) {
		fprintf(stderr, "Cannot calibrate camera with no chessboard images!\n");
		return false;
	}

	finder->generateMatrices();

	cvCalibrateCamera2(
		// In
		finder->objectPoints,
		finder->imagePoints,
		finder->pointCounts,
		imageSize,

		// Out
		intrinsic,
		distortion,
		rotation,		// TODO: Optional
		translation,	// TODO: Optional

		flags
	);

	return true; // XXX: TEMP--No real status here
}

} // end namespace Calibration
} // end namespace Cv
