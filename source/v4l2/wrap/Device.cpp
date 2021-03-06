#include "Device.hpp"
#include <stdio.h>
#include <fcntl.h> // open
#include <unistd.h> // close
#include <stropts.h> // ioctl
#include <linux/videodev2.h> // v4l2

namespace V4L2 {

Device::Device(const std::string& fName):
	name(fName),
	fd(0),
	streaming(false)
{
	// Nothing
}

Device::~Device()
{
	if(isOpen()) {
		streamOff();
		close();
	}
}

bool Device::open()
{
	if(isOpen()) {
		return false;
	}

	fd = ::open(name.c_str(), O_RDWR);
	if(fd == -1) {
		// TODO: Report error.
		fprintf(stderr, 
			"Device::open() err: could not open device %s.\n", name.c_str());
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
	if(isStreaming()) {
		streamOff();
	}
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
	if(!isOpen()) {
		open();
	}
	if(isStreaming()) {
		return true;
	}

	v4l2_buf_type buffType = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(ioctl(fd, VIDIOC_STREAMON, &buffType) == -1) {
		fprintf(stderr, "Could not turn stream on.\n");
		return false;
	}
	streaming = true; 
	return true;
}

bool Device::streamOff()
{
	if(!isStreaming()) {
		return true;
	}
	v4l2_buf_type buffType = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(ioctl(fd, VIDIOC_STREAMOFF, &buffType) == -1) {
		fprintf(stderr, "Could not turn stream off.\n");
		return false;
	}
	streaming = false;
	return true;
}

} // end namespace V4L2
