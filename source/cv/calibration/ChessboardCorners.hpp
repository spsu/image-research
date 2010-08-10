#ifndef BT_Cv_Calibration_ChessboardCorners
#define BT_Cv_Calibration_ChessboardCorners

/**
 * Copyright (c) 2010 Brandon Thomas Suit
 * http://possibilistic.org | echelon@gmail.com
 * Code available for use under the LGPL 2.
 * 
 * Cv::Calibration::ChessboardCorners
 *   Simple container for corners detected in the calibration process. 
 *   TODO: A work in progress. 
 */

#include <cv.h>

namespace Cv {
	namespace Calibration {
		class ChessboardFinder;
	}
}

namespace Cv {
namespace Calibration {
class ChessboardCorners
{
	friend class ChessboardFinder;

	public:
		// CTOR. Board size only. Corners instantiated.
		ChessboardCorners(CvSize board);

		// CTOR. Takes ownership of corners supplied.
		ChessboardCorners(CvSize board, CvPoint2D32f* crns);

		// DTOR
		~ChessboardCorners();

		// Accessor. Caller doesn't get ownership!
		CvPoint2D32f* getCorners() { return corners; };

		// Accessor.
		int getNumFound() { return numFound; };

		// Determine if all board corners found. 
		bool allFound() { 
			return (numFound == (boardSize.width*boardSize.height)); 
		};

	protected:
		// Board size
		CvSize boardSize;

		// Corners
		CvPoint2D32f* corners;

		// Number found
		int numFound;
};
}
}

#endif
