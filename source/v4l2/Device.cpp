#include "Device.hpp"
#include "Capabilities.hpp"
#include <stdio.h>
#include <fcntl.h> // open
#include <unistd.h> // close
#include <stropts.h> // ioctl
#include <linux/videodev2.h> // v4l2_capability struct

namespace V4L2 {

Device::Device(const std::string& fName):
	name(fName),
	fd(0),
	cap(0)
{
	// Nothing
}

Device::~Device()
{
	if(isOpen()) {
		close();
	}
	if(cap != NULL) {
		delete cap;
	}
}

bool Device::open()
{
	if(isOpen()) {
		fprintf(stderr, "Device::open() err: device is already open\n");
		return false;
	}

	printf("Opening %s\n", name.c_str());
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

Capabilities* Device::getCapabilities()
{
	if(cap == NULL) {
		cap = new Capabilities(this);
	}
	return cap;
}

} // end namespace V4L2
