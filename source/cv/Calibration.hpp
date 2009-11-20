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
		 */		
		Calibration(int boardW = 0, int boardH = 0, int num = 7);

		/**
		 * Calibration DTOR.
		 */
		~Calibration();

		/**
		 * Set the board finding parameters.
		 */
		void setBoardParams(int boardW, int boardH, int num);

		/**
		 * If the object calibration is ready for processing images.
		 */
		bool isCalibrated() { return calibrated; };

		/**
		 * Load a calibration XML file.
		 * XXX TODO: Make this a CTOR instead...
		 */
		//bool loadFiles(std::string intrinsicsFile, std::string distortionFile);

		/**
		 * Save a calibration XML file.
		 */
		//bool saveFiles(std::string intrinsicsFile, std::string distortionFile);

		/**
		 * An iteration to find a calibration board.
		 * Returns true if found.
		 */
		bool findBoardIter(Image* im);
		void drawBoardIter(Image* im);
		bool findAndDrawBoardIter(Image* im); 

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
		CvSize size;

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
};
}

#endif
