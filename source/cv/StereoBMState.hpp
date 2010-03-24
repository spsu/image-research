#ifndef BT_Cc_StereoBMState
#define BT_Cv_StereoBMState

/**
 * Copyright (c) 2010 Brandon Thomas Suit
 * http://possibilistic.org | echelon@gmail.com
 * Code available for use under the LGPL 2.
 * 
 * Cv::StereoBMState
 *   Manages an internal OpenCV Stereo BM state and finds stereo correspondence 
 *   between two stereo images. 
 */

#include <cv.h>
#include <highgui.h>

namespace Cv {
	//class Arr; // TODO Arr->Mat->IplImage OO design
	class Image;
}

namespace Cv {
class StereoBMState
{
	public:
		/**
		 * CTOR.
		 */
		StereoBMState(int presetFlag = CV_STEREO_BM_BASIC, 
					  int numberOfDisparities = 0);

		/**
		 * DTOR.
		 */
		~StereoBMState();

		/**
		 * Finds correspondence between two stereo images. 
		 * Supply the left and right images, and the result will be calculated.
		 * TODO: Make left and right const!
		 */
		void findCorrespondence(Image* left, Image* right, Image* ret);

		/**
		 * Return pointer to BMState.
		 * Ownership should remain with this object. 
		 */
		CvStereoBMState* getPtr() { return state; };

	private:
		/**
		 * OpenCV struct
		 */
		CvStereoBMState* state;

};
}

#endif
