#ifndef BT_Cv_Calibration_StereoData
#define BT_Cv_Calibration_StereoData

namespace Cv {
namespace Calibration {
class StereoData
{
	public:
		StereoData();
		~StereoData();

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
