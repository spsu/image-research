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
}

namespace Cv {
class ChessboardCorners
{
	public:
		/**
		 * CTOR.
	 	 */
		ChessboardCorners(CvSize sz);
		ChessboardCorners(int width, int height);

		/**
		 * DTOR.
		 */
		~ChessboardCorners();

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

	protected:
		/**
		 * Size of the board.
		 */
		CvSize patternSize;

		/**
		 * Vector of all corners recorded.
		 */
		std::vector<CvPoint2D32f*> allCorners;
};
}

#endif
