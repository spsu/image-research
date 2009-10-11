#include "Camera.hpp"
#include "Image.hpp"
#include <stdio.h>

namespace Cv {

Camera::Camera(int device):
	deviceNum(device),
	width(0),
	height(0),
	frameCache(0)
{
	capture = cvCreateCameraCapture(device);
	if (!capture) {
		fprintf(stderr, "Camera CTOR: Could not use device %d.\n", device);
	}

	width = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
	height = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
}

Camera::~Camera()
{
	cvReleaseCapture(&capture);

	if(frameCache != 0) {
		delete frameCache;
	}
}

void Camera::setSize(int w, int h)
{
	// TODO: The following doesn't seem to work. OpenCV build issue?
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, (double)width);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, (double)height);
}

void Camera::setFps(int fps)
{
	cvSetCaptureProperty(capture, CV_CAP_PROP_FPS, (double)fps);
}

/**
 * Do not deallocate the IplImage* returned. 
 * The image is not owned by caller.
 */
Image* Camera::queryFrame()
{
	IplImage* frame = 0; // Never deallocate!
	Image* newImg = 0;

	frame = cvQueryFrame(capture); // capture device deallocates on call!!
	newImg = new Image(frame, true);

	if(frameCache != 0) {
		delete frameCache;
	}
	frameCache = newImg;
	return newImg;
}

int Camera::getWidth() 
{
	return width;
}

int Camera::getHeight()
{
	return height;
}

} // end namespace Cv
