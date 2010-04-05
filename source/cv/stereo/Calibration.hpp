#ifndef BT_Cv_Stereo_Calibration
#define BT_Cv_Stereo_Calibration

#include <cv.h>

namespace Cv {
	namespace Stereo {
		class Rectification;
	}
}

namespace Cv {
namespace Stereo {
class Calibration
{
	// Rectification uses Stereo::Calibration data.
	friend class Rectification;

	public:
		/**
		 * CTOR.
		 */
		Calibration();

		/**
		 * DTOR.
		 */
		~Calibration();

		bool calibrate();

	protected:
		// Rotation & translation relating **the two cameras**
		// (cf. Intrinsics' rotation & translation, wherein relation is between
		//     the camera and each image of the chessboard)
		CvMat* rotation;	// (3x3)
		CvMat* translation; // (3x1)

		// (3x3) Physical geometry info relating the two cameras in 3D space.
		// (ie. rotation, translation.) NOT pixel coords!
		CvMat* essential;

		// (3x3) Pixel info relating the two cameras in terms of pixel space.
		// Utilizes intrinics of both cams.
		CvMat* fundamental;
};
}
}

#endif
