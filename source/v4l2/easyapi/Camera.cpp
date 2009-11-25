#include "Camera.hpp"
#include "Buffers.hpp"
#include "Frame/DriverFrame.hpp"
#include "../wrap/Capability.hpp"
#include "../wrap/Format.hpp"
#include <stdio.h>

namespace V4L2 {

Camera::Camera(const std::string& devicePath):
	Device(devicePath),
	capability(NULL),
	format(NULL),
	buffers(NULL)
{
	capability = new Capability(this);
	format = new Format(this);
}

Camera::~Camera()
{
	if(capability != NULL) {
		delete capability;
	}
	if(format != NULL) {
		delete format;
	}
}

void Camera::printInfo()
{
	capability->printAll();
	printf("\n");
	format->printAll();
	if(isOpen()) {
		printf("\nUsing file descriptor: %d\n", fd);
	}

}

Capability* Camera::getCapability()
{
	if(capability == NULL) {
		capability = new Capability(this);
	}
	return capability;
}

Format* Camera::getFormat()
{
	if(format == NULL) {
		format = new Format(this);
	}
	return format;
}

void Camera::setupBuffers()
{
	buffers = new Buffers(this);
	if(!buffers->initBuffers()) {
		fprintf(stderr, "Could not init buffers\n");
		return;
	}
}

DriverFrame* Camera::grabTemporaryFrame()
{
	if(!isStreaming() && !streamOn()) {
		fprintf(stderr, 
			"Camera could not grab frame: streaming could not be turned on.\n");
		return NULL;
	}
	if(buffers == NULL) {
		setupBuffers();
	}
	return buffers->grabTemporaryFrame();
}

bool Camera::dequeue()
{
	if(buffers == NULL) {
		setupBuffers();
	}
	return buffers->dequeue();
}

bool Camera::dequeueOne()
{
	if(buffers == NULL) {
		setupBuffers();
	}
	return buffers->dequeueOne();
}

} // end namespace V4L2
