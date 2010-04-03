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

	protected:
		// Single rotation & translation vector relating the two cameras
		// cf. CamData.rotation & translation, wherein they relate the camera
		//     and each image of the chessboard
		CvMat* rotation;
		CvMat* translation;


		// Physical geometry info relating the two cameras in 3D space.
		// (ie. rotation, translation.) NOT pixel coords!
		CvMat* essential;

		// Pixel info relating the two cameras in terms of pixel space.
		// Utilizes intrinics of both cams.
		CvMat* fundamental;


};
}
}

#endif
