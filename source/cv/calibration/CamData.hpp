#ifndef BT_Cv_Calibration_CamData
#define BT_Cv_Calibration_CamData

// TODO: Make ChessboardCorners a friend class
// TODO: Rename this file CamData.hpp.
namespace Cv {
namespace Calibration {
class CamData
{
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
