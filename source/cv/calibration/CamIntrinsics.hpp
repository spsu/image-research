#ifndef BT_Cv_Calibration_CamIntrinsics
#define BT_Cv_Calibration_CamIntrinsics

#include <cv.h>

namespace Cv {
	namespace Stereo {
		class Rectification;
		class Calibration;
	}
}

// TODO: Make ChessboardCorners a friend class
namespace Cv {
namespace Calibration {
class CamIntrinsics
{
	// Utilize CamIntrinsics to compute
	friend class Stereo::Rectification;
	friend class Stereo::Calibration;

	public:
		/**
		 * CTORs.
		 */
		CamIntrinsics(int width, int height, int boards);
		CamIntrinsics(CvSize boardSize, int boards);

		/**
		 * DTOR.
		 */
		~CamIntrinsics();

		/**
		 * Calibrate camera based on chessboard points.
		 */
		bool calibrateCam(ChessboardFinder* finder);

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
