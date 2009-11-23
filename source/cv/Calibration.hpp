#ifndef CV_CALIBRATION
#define CV_CALIBRATION

#include <cv.h>

namespace Cv {
	class Image;
}

namespace Cv {
class Calibration
{
	public:
		/**
		 * Calibration CTOR.
		 * Supply the chessboard size and the number of boards to find.
		 * Setting the number of boards to find is optional.
		 */		
		Calibration(int boardW, int boardH, int num = 0);

		/**
		 * Calibration DTOR.
		 */
		~Calibration();

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
		bool isCalibrated() { return calibrated; };

		/**
		 * Load calibration matrices from XML files.
		 * TODO: Provide an additional CTOR to do this.
		 */
		bool loadIntrinsics(std::string filename);
		bool loadDistortion(std::string filename);

		/**
		 * Generate maps for undistortion after configs are loaded.
		 */
		bool doGenerateMap();

		/**
		 * Save calibration matrices in XML files.
		 */
		bool saveIntrinsics(std::string filename);
		bool saveDistortion(std::string filename);

		/**
		 * An iteration to find a calibration board.
		 * Returns true if found.
		 */
		bool findBoardIter(Image* im);
		void drawBoardIter(Image* im);
		bool findAndDrawBoardIter(Image* im); 

		/**
		 * Return the number of found boards thus far.
		 */
		int getNumFound();

		/**
		 * Undistort an image, once calibrated.
		 */
		bool undistort(Image* im);

		/**
		 * Make a new undistorted image.
		 */
		//Image* getUndistorted(Image* im);

	protected:
		/**
		 * The camera intrinsics matrix.
		 */
		CvMat* intrinsics;

		/**
		 * The camera distortion matrix.
		 */
		CvMat* distortion;

		/**
		 * Distortion map.
		 * This is applied to the final, corrected image. 
		 */
		IplImage* xMap;
		IplImage* yMap;

		/**
		 * The calibration board size.
		 */
		CvSize boardSize;

		/**
		 * The camera image frame size.
		 */
		CvSize frameSize;

		/**
		 * The number of completely identified calibration boards to find.
		 */
		int numToFind;

		/**
		 * The number of boards found.
		 */
		int numFound;

		/**
		 * Calibration state.
		 */
		bool calibrated;

		/**
		 * Nested TempData class.
		 * This information is reused in each iteration of board finding, but
		 * not elsewhere. 
		 */
		struct BoardIterData {
			IplImage* gray;
			CvMat* imgPts;
			CvMat* objPts;
			CvMat* ptCounts;
			CvPoint2D32f* corners;
			int cornerCnt;
			int found;
			int area;
			int step;
			/**
			 * CTOR.
		 	 */
			BoardIterData(): 
				gray(0), imgPts(0), objPts(0), ptCounts(0),
				corners(0), cornerCnt(0), found(0), area(0), step(0) {};
		};

		/**
		 * Class instance of BoardIterData.
		 */
		BoardIterData* iterData;

		/**
		 * Generate the intrinsics matricies.
		 */
		bool generateIntrinsics();

		/**
		 * Generate the undistort maps.
		 */
		bool generateMap();
};
}

#endif
