#include "ChessboardCorners.hpp"

namespace Cv {
namespace Calibration {

ChessboardCorners::ChessboardCorners(CvSize board):
	boardSize(board),
	corners(0),
	numFound(0)
{
	corners = new CvPoint2D32f[boardSize.width * boardSize.height];
}

ChessboardCorners::ChessboardCorners(CvSize board, CvPoint2D32f* crns):
	boardSize(board),
	corners(crns),
	numFound(0)
{
	// Nothing
}

ChessboardCorners::~ChessboardCorners()
{
	if(corners != NULL) {
		delete corners;
	}
}

} // end namespace Calibration
} // end namespace Cv
