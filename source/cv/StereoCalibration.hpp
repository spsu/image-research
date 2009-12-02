#ifndef Cv_StereoCalibration
#define Cv_StereoCalibration

#include <cv.h>

namespace Cv {

class Image;

/**
 * State of the StereoCalibration class.
 * Aside from *_DONE/LOADED (which imply *almost* the same thing), these states 
 * follow in order from top to bottom, the final state(s) ultimately 
 * representing completion. 
 */
enum StereoState{
	STEREO_INSTANTIATED,
	STEREO_CALIBRATION_RUNNING,
	STEREO_CALIBRATION_DONE,
	STEREO_CALIBRATION_LOADED,		// If loaded via XML
	STEREO_RECTIFICATION_DONE,
	STEREO_RECTIFICATION_LOADED,
};

/**
 * Set up a stereo camera system
 * This enables calibration (undistortion), rectification, and correspondance.
 * The calculated values may then be saved to prevent later unnecessary 
 * recalibrations.
 *
 * Much of the code in this class is inspired by the code in the following text: 
 *			"Learning OpenCV" [Bradski & Kaehler 2008], published by O'Reilly. 
 *				* Chapter 11: Camera Models and Calibration, pg 398 - 401
 *				* Chapter 12: Projection and 3D Vision, pg 446 - 452
 */
class StereoCalibration
{
	public:
		/**
		 * StereoCalibration CTOR.
		 * Supply the chessboard size and the number of boards to find.
		 * Setting the number of boards to find is optional.
		 */		
		StereoCalibration(int boardW, int boardH, int num = 0);

		/**
		 * Calibration DTOR.
		 */
		~StereoCalibration();

		/**
		 * Set the board finding parameters.
		 * This is NECESSARY, but can also be done from the CTOR.
		 */
		void setBoardParams(int boardW, int boardH, int num);

		/**
		 * Set the camera image frame size.
		 * This is NECESSARY. 
		 */
		void setCameraFrameSize(int width, int height);

		/**
		 * If the object calibration is ready for processing images.
		 */
		//bool isCalibrated() { return calibrated; };


		/**
		 * Generate maps for undistortion after configs are loaded.
		 */
		bool doGenerateMap();


		/**
		 * An iteration to find a calibration board.
		 * Returns true if found.
		 */
		bool findBoardsIter(Image* im1, Image* im2);
		void drawBoardIter(Image* im1, Image* im2);
		bool findAndDrawBoardIter(Image* im1, Image* im2); 

		/**
		 * Return the number of found boards thus far.
		 */
		int getNumFound();

		/**
		 * Remap to the rectified state.
		 * The supplied image is overwritten. 
		 */
		bool remap1(Image* im);
		bool remap2(Image* im);

	protected:
		// ===================== PROTECTED MEMBERS ========================== //

		/**
		 * State of the stereo calibration. 
		 */
		StereoState stereoState;

		/**
		 * The camera intrinsics and distortion matrices.
		 * Calculated by cvStereoCalibrate() or cvCalibrateCamera2().
		 */
		CvMat* intrinsics1;
		CvMat* intrinsics2;
		CvMat* distortion1;
		CvMat* distortion2;

		/**
		 * The stereo parameters.
		 * Calculated by cvStereoCalibrate().
		 */
		CvMat* rotation;
		CvMat* translation;	
		CvMat* essential;
		CvMat* fundamental;

		/**
		 * Rectification parameters.
		 * Calculated by cvStereoRectify(). 
		 */
		CvMat* rotation1;
		CvMat* rotation2;
		CvMat* projection1;
		CvMat* projection2;
		CvMat* reprojection; // AKA "Q"

		/**
		 * Calibration data
		 */
		CvSize boardSize;	// Calibration board size
		CvSize frameSize;	// Camera image frame size
		int numToFind; 		// Number of boards to identify
		int numFound;		// Number of boards found thus far

		/**
		 * Nested TempData class.
		 * This information is reused in each iteration of board finding, but
		 * not elsewhere. 
		 */
		struct TempData {
			IplImage* gray1;
			IplImage* gray2;
			CvMat* objPts;
			CvMat* imgPts1;
			CvMat* imgPts2;
			CvMat* ptCounts;
			CvPoint2D32f* corners1;
			CvPoint2D32f* corners2;
			int cornerCnt1;
			int cornerCnt2;
			int area;
			int step;
			/**
			 * CTOR.
		 	 */
			TempData(): 
				gray1(0), gray2(0), objPts(0), imgPts1(0), imgPts2(0),
				ptCounts(0), corners1(0), corners2(0), 
				cornerCnt(0), cornerCnt(0), area(0), 
				step(0) {};
		};

		/**
		 * Class instance of TempData.
		 */
		TempData* tempData;

		// ===================== PROTECTED METHODS ========================== //

		/**
		 * Generate the intrinsics matricies.
		 */
		bool generateIntrinsics();

		/**
		 * Generate the undistort maps.
		 */
		bool generateMap();

		/**
		 * Set up all of the 13 matrices used in stereo setup. 
		 */
		void setupMatrices();
};

}

#endif
