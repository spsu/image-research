#ifndef CV_CAMERA
#define CV_CAMERA

#include <cv.h>
#include <highgui.h>

namespace Cv {
	class Image;
}

/**
 * XXX Note: Based on my robot project's Vision::Camera class. I removed the 
 * calibration components (and may want them again at some point in the future)
 */
namespace Cv {
class Camera 
{
	public:
		/**
		 * Camera CTOR.
		 * Specify the camera number to query.
		 */
		Camera(int device = 0);

		/**
		 * DTOR.
		 */
		~Camera();
		
		/**
		 * Set the camera dimensions.
		 * XXX: The cvSetCaptureProperty doesn't seem to work. (WHY??)
		 */
		void setSize(int w, int h);
		
		/**
		 * Set the FPS
		 */
		void setFps(int fps);

		/**
		 * Grab and retrieve the image from the camera.
		 * XXX: Very important :
		 * Do not release or modify the returned Image (just like the returned
		 * IplImage in unwrapped OpenCV). This class will take care of all
		 * deallocation!
		 */
		Image* queryFrame();
		
		/**
		 * Return the original dimensions of the frame, or the resized
		 * dimensions.
		 */
		int getWidth();
		int getHeight();

	protected:
		/**
		 * Capture structure.
		 */
		CvCapture* capture;

		/**
		 * Device number.
		 */
		int deviceNum;
		
		/**
		 * Default resolution.
		 */
		int width;
		int height;

		/**
		 * Cached image. Automatically deleted.
		 */
		Image* frameCache;

};
}

#endif
