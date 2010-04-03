#ifndef BT_Cv_Calibration_StereoRectification
#define BT_Cv_Calibration_StereoRectification

namespace Cv {
namespace Calibration {
class StereoRectification
{
	public:
		/**
		 * CTOR.
		 */
		StereoRectification();

		/**
		 * DTOR.
		 */
		~StereoRectification();

	protected:
		// (3x3) Rectification rotations
		CvMat* rotationLeft;		
		CvMat* rotationRight;

		// (3x4) Rectification projections
		CvMat* projectionLeft;		
		CvMat* projectionRight;

		// (4x4) Rectification reprojection
		CvMat* reprojection;

};
}
}
