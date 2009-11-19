#include "Camera.hpp"
#include "../wrap/Capability.hpp"
#include "../wrap/Format.hpp"
#include <stdio.h>

namespace V4L2 {

Camera::Camera(const std::string& devicePath):
	Device(devicePath),
	capability(NULL),
	format(NULL)
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


} // end namespace V4L2
