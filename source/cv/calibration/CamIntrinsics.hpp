#ifndef BT_Cv_Calibration_CamIntrinsics
#define BT_Cv_Calibration_CamIntrinsics

#include <cv.h>

namespace Cv {
	namespace Stereo {
		class Rectification;
	}
}

// TODO: Make ChessboardCorners a friend class
namespace Cv {
namespace Calibration {
class CamIntrinsics
{
	// Utilizes CamIntrinsics to compute
	friend class Rectification;

	public:
		/**
		 * CTORs.
		 */
		Data(int width, int height, int numBoards);
		Data(CvSize boardSize, int numBoards);

		/**
		 * DTOR.
		 */
		~Data();

		/**
		 * Accessors.
		 */
		CvMat* getImagePts() { return imgPts; };
		CvMat* getObjectPts() { return objPts; };

	protected:
		/**
		 * Describes the sizes and number of boards.
		 */
		CvSize imageSize;
		CvSize boardSize;
		int numBoards;

		/**
		 * Harvested image points for a calibrated camera.
		 */
		CvMat* imgPts;

		/**
		 * Harvested object points for a calibrated camera.
		 */
		CvMat* objPts; // (Nx3? p429)

		/* ===================== CALIBRATION DATA ========================= */

		/**
		 * Camera Intrinsics.
		 */
		CvMat* intrinsic;	// (3x3) Describe the Ideal camera behavior.
		CvMat* distortions;	// (5x1) Describe the Non-ideal camera behavior.

		/**
		 * Camera Extrinsics.
		 * ie. where the calibration objects were found relative to the camera.
		 */
		CvMat* rotation;	// (Nx3) Axis each chessboard was rotated on
		CvMat* translation; // (Nx3) Translation of each chessboard

};
}
}
