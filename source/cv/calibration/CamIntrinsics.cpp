#include "CamIntrinsics.hpp"
#include "ChessboardFinder.hpp"

namespace Cv {
namespace Calibration {

CamIntrinsics::CamIntrinsics(int imWidth, int imHeight):
	imageSize(cvSize(imWidth, imHeight)),
	imagePoints(0),
	intrinsics(0),
	distortion(0),
	rotation(0),
	translation(0)
{
	// Nothing yet
}

CamIntrinsics::CamIntrinsics(CvSize imgSize):
	imageSize(imgSize),
	imagePoints(0),
	intrinsics(0),
	distortion(0),
	rotation(0),
	translation(0)
{
	// Nothing yet
}

CamIntrinsics::~CamIntrinsics()
{
	if(imagePoints != NULL) {
		cvReleaseMat(&imagePoints);
	}

	if(intrinics != NULL) {
		cvReleaseMat(&intrinsics);
		cvReleaseMat(&distortion);
	}
	
	if(rotation != NULL) {
		cvReleaseMat(&rotation);
		cvReleaseMat(&translation);
	}
}

bool CamIntrinsics::calibrateCam(ChessboardFinder* finder)
{
	int numFound = 0;
	int flags = 0;	

	cvCalibrateCamera2(
		// In
		finder->getObjectPoints(),
		finder->getImagePoints(),
		finder->getPointCounts(),
		imageSize,

		// Out
		intrinsics,
		distortion,
		rotation,		// TODO: Optional
		translation,	// TODO: Optional

		flags
	);
}

} // end namespace Calibration
} // end namespace Cv
