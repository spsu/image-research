#include "Device.hpp"
#include <stdio.h>
#include <fcntl.h> // open
#include <unistd.h> // close

namespace V4L2 {

Device::Device(const std::string& fName):
	name(fName),
	handle(0)
{
	// Nothing
}

Device::~Device()
{
	if(isOpen()) {
		close();
	}
}

bool Device::open()
{
	if(isOpen()) {
		fprintf(stderr, "Device::open() err: device is already open\n");
		return false;
	}

	printf("Opening %s\n", name.c_str());
	handle = ::open(name.c_str(), O_RDWR);
	if(handle == -1) {
		// TODO: Report error.
		fprintf(stderr, "Device::open() err: could not open device\n");
		handle = 0;
		return false;
	}
	return true;
}

bool Device::isOpen()
{
	return bool(handle > 0);
}

void Device::close()
{
	::close(handle);
	handle = 0;
}


} // end namespace V4L2
