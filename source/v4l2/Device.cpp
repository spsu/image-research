#include "Device.hpp"
#include "Capability.hpp"
#include "Format.hpp"
#include <stdio.h>
#include <fcntl.h> // open
#include <unistd.h> // close
#include <stropts.h> // ioctl
#include <linux/videodev2.h> // v4l2

namespace V4L2 {

Device::Device(const std::string& fName):
	name(fName),
	fd(0),
	capability(0),
	format(0)
{
	// Nothing

	capability = new Capability(this);
	format = new Format(this);
}

Device::~Device()
{
	if(isOpen()) {
		streamOff();
		close();
	}
	if(capability != NULL) {
		delete capability;
	}
	if(format != NULL) {
		delete format;
	}
}

bool Device::open()
{
	if(isOpen()) {
		fprintf(stderr, "Device::open() err: device is already open\n");
		return false;
	}

	fd = ::open(name.c_str(), O_RDWR);
	if(fd == -1) {
		// TODO: Report error.
		fprintf(stderr, "Device::open() err: could not open device\n");
		fd = 0;
		return false;
	}
	return true;
}

bool Device::isOpen()
{
	return bool(fd > 0);
}

void Device::close()
{
	::close(fd);
	fd = 0;
}

int Device::getFd()
{
	open();
	return fd;
}

bool Device::streamOn()
{
	v4l2_buf_type buffType = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(ioctl(fd, VIDIOC_STREAMON, &buffType) == -1) {
		fprintf(stderr, "Could not turn stream on.");
		return false;
	}
	return true;
}

bool Device::streamOff()
{
	v4l2_buf_type buffType = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(ioctl(fd, VIDIOC_STREAMOFF, &buffType) == -1) {
		fprintf(stderr, "Could not turn stream off.");
		return false;
	}
	return true;
}

void Device::printInfo()
{
	capability->printAll();
	printf("\n");
	format->printAll();
	if(isOpen()) {
		printf("\nUsing file descriptor: %d\n", fd);
	}
}

Capability* Device::getCapability()
{
	if(capability == NULL) {
		capability = new Capability(this);
	}
	return capability;
}

Format* Device::getFormat()
{
	if(format == NULL) {
		format = new Format(this);
	}
	return format;
}

} // end namespace V4L2
