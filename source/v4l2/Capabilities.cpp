#include "Capabilities.hpp"
#include "Device.hpp"
#include <stropts.h> // ioctl
#include <stdio.h>

namespace V4L2 {

Capabilities::Capabilities(Device* dev)
{
	device = dev;
}

Capabilities::~Capabilities()
{
	// TODO: Destroy capabilities struct
}

const char* Capabilities::driver()
{
	doQuery();
	return (const char*)cap.driver;
}

const char* Capabilities::card()
{
	doQuery();
	return (const char*)cap.card;
}

const char* Capabilities::busInfo()
{
	doQuery();
	return (const char*)cap.bus_info;
}

int Capabilities::version()
{
	doQuery();
	return (int)cap.version;
}

bool Capabilities::hasVideoCapture()
{
	doQuery();
	return bool(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE);
}

bool Capabilities::hasReadWrite()
{
	doQuery();
	return bool(cap.capabilities & V4L2_CAP_READWRITE);
}

bool Capabilities::hasStreaming()
{
	doQuery();
	return bool(cap.capabilities & V4L2_CAP_STREAMING);
}

/*  TODO
   Don't have a doQuery method. Do this in the CTOR and throw an except.
   if it fails. Have Device->getCapabilities() catch the exception and 
   return null if failure. */
bool Capabilities::doQuery() 
{
	int ret = 0;

	if(queried) {
		return false;
	}
	queried = true;

	ret = ioctl(device->fd, VIDIOC_QUERYCAP, &cap);
	if(ret != 0) {
		fprintf(stderr, "There was an error in querying the camera.\n");
		return false;
	}
	return true;
}

} // end namespace V4L2
