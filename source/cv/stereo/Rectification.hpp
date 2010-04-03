#ifndef BT_Cv_Stereo_Rectification
#define BT_Cv_Stereo_Rectification

#include <cv.h>

namespace Cv {
	namespace Stereo {
		class Calibration;
	}
}

namespace Cv {
namespace Stereo {
class Rectification
{
	public:
		/**
		 * CTOR.
		 */
		Rectification();

		/**
		 * DTOR.
		 */
		~Rectification();

		/**
		 * Perform rectification with two Stereo::Calibration objects.
		 * TODO: (Check that they are already calibrated.)
		 */
		bool rectify(Calibration* calib1, Calibration* calib2, int flags = 0);

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
