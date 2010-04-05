#include "CamIntrinsics.hpp"
#include "ChessboardFinder.hpp"

namespace Cv {
namespace Calibration {

CamIntrinsics::CamIntrinsics(int bWidth, int bHeight, int boards):
	imageSize(cvSize()),
	boardSize(cvSize(bWidth, bHeight)),
	numBoards(boards),
	imagePoints(0),
	intrinsics(0),
	distortion(0),
	rotation(0),
	translation(0)
{
	// Nothing yet
}

CamIntrinsics::CamIntrinsics(CvSize boardSize, int boards):
	imageSize(cvSize()),
	boardSize(boardSize),
	numBoards(boards),
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
	CvMat* objPts = 0;
	CvMat* imgPts = 0;
	CvMat* pointCounts = 0;
	int numFound = 0;

	int flags = 0;	

	numFound = finder->numFound();

	objPts = finder->getObjectPoints();
	imgPts = finder->getImagePoints();

	cvCalibrateCamera2(
		// In
		objPts,
		imgPts,
		pointCounts,
		imageSize,

		// Out
		intrinsics,
		distortion,
		rotation,
		translation,

		flags
	);
}

} // end namespace Calibration
} // end namespace Cv
