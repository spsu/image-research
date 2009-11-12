#include "Capability.hpp"
#include "Device.hpp"
#include <stropts.h> // ioctl
#include <stdio.h>
#include <string.h> // memset

namespace V4L2 {

Capability::Capability(Device* dev)
{
	device = dev;
	reset();
}

Capability::~Capability()
{
	// TODO: Destroy capabilities struct
	reset();
}

void Capability::reset()
{
	memset(&capability, 0, sizeof(capability));
}

const char* Capability::driver()
{
	doQuery();
	return (const char*)capability.driver;
}

const char* Capability::card()
{
	doQuery();
	return (const char*)capability.card;
}

const char* Capability::busInfo()
{
	doQuery();
	return (const char*)capability.bus_info;
}

int Capability::version()
{
	doQuery();
	return (int)capability.version;
}

bool Capability::hasVideoCapture()
{
	doQuery();
	return bool(capability.capabilities & V4L2_CAP_VIDEO_CAPTURE);
}

bool Capability::hasReadWrite()
{
	doQuery();
	return bool(capability.capabilities & V4L2_CAP_READWRITE);
}

bool Capability::hasStreaming()
{
	doQuery();
	return bool(capability.capabilities & V4L2_CAP_STREAMING);
}

/*  TODO
   Don't have a doQuery method. Do this in the CTOR and throw an except.
   if it fails. Have Device->getCapability() catch the exception and 
   return null if failure. */
bool Capability::doQuery() 
{
	int ret = 0;

	if(queried) {
		return false;
	}
	queried = true;
	reset();
	ret = ioctl(device->fd, VIDIOC_QUERYCAP, &capability);
	if(ret != 0) {
		fprintf(stderr, "There was an error in querying the camera.\n");
		return false;
	}
	return true;
}

} // end namespace V4L2
