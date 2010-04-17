#ifndef BT_Cv_Calibration_ChessboardFinder
#define BT_Cv_Calibration_ChessboardFinder

/**
 * Copyright (c) 2010 Brandon Thomas Suit
 * http://possibilistic.org | echelon@gmail.com
 * Code available for use under the LGPL 2.
 * 
 * Cv::Calibration::ChessboardFinder
 *   An easy mechanism for finding and recording chessboard corners in an
 *   image. 
 *   TODO: A work in progress. 
 */

#include <cv.h>
#include <vector>

namespace Cv {
	class Image;
	namespace Calibration {
		class CamIntrinsics;
	}
	namespace Stereo {
		class Calibration;
	}
}

namespace Cv {
namespace Calibration {
class ChessboardFinder
{
	// For cvCalibrateCamera2
	friend class CamIntrinsics;
	friend class Stereo::Calibration; // TODO: Remove this class from API 

	public:
		/**
		 * CTOR.
	 	 */
		ChessboardFinder(CvSize sz);
		ChessboardFinder(int width, int height);

		/**
		 * DTOR.
		 */
		~ChessboardFinder();

		/**
		 * Find corners in an image and add them to the points vector.
		 * Returns true if corners were found, false if not. 
		 */
		bool findCorners(Cv::Image* img, 
						 int flags = CV_CALIB_CB_ADAPTIVE_THRESH);

		/**
		 * Return the number of corners found thus far.
		 */
		int numFound();

		/**
		 * Generate the object points, point counts, etc. from the data we have. 
		 * Only compiled if the "dirty" flag is set, which is only if new frames
		 * have been captured since last calc'd. 
		 * TODO: Specify the measurements. (see book p. ??)
		 */
		void generateMatrices();

		/**
		 * Get the object points.
		 * TODO: Specify measurements.
		 */
		CvMat* getObjectPoints();
		CvMat* getImagePoints();
		CvMat* getPointCounts();

	protected:
		/**
		 * Size of the board.
		 */
		CvSize boardSize;
		int boardArea;

		/**
		 * Vector of all corners recorded.
		 */
		std::vector<CvPoint2D32f*> allCorners;

		/**
		 * Data to be calculated.
		 */
		CvMat* objectPoints; // (Nx3) Harvested object points (theoretical).
		CvMat* imagePoints;  // (Nx2) Harvested physical image points.
		CvMat* pointCounts;	 // (1xN) Number of points in each frame


	private:
		// Flag for generated matricies. 
		// If dirty, must recalculate them. 
		bool matricesDirty;

		// Temporary grayscale image. 
		// Don't want to waste time alloc/dealloc over and again. 
		IplImage* tempGray;

};
}
}

#endif
