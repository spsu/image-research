#include "Capability.hpp"
#include "Device.hpp"
#include <stropts.h> // ioctl
#include <stdio.h>
#include <string.h> // memset

namespace V4L2 {

Capability::Capability(Device* dev):
	queried(false)
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

void Capability::printAll()
{
	// A lot of information to output...
	printf(
		"Basic Info:\n"		\
		"    Driver: %s\n"	\
		"    Card: %s\n"	\
		"    Bus info: %s\n"\
		"    Version: %2d\n"\
		"\nCapabilities:\n"	\
		"    video capture: %s\n"	\
		"    video overlay: %s\n"	\
		"    VBI capture: %s\n"		\
		"    VBI output: %s\n"		\
		"    sliced VBI capture: %s\n"\
		"    sliced VBI output: %s\n"\
		"    RDS capture: %s\n"		\
		"    video ouput overlay: %s\n"\
		"    has tuner: %s\n"		\
		"    has audio: %s\n"		\
		"    has radio: %s\n"		\
		"    async IO: %s\n"		\
		"    streaming: %s\n",
		
		driver(),
		card(),
		busInfo(),
		version(),
		hasVideoCapture()? "YES" : "no",
		hasVideoOverlay()? "YES" : "no",
		hasVbiCapture()? "YES" : "no",
		hasVbiOutput()? "YES" : "no",
		hasSlicedVbiCapture()? "YES" : "no",
		hasSlicedVbiOutput()? "YES" : "no",
		hasRdsCapture()? "YES" : "no",
		hasVideoOutputOverlay()? "YES" : "no",
		hasTuner()? "YES" : "no",
		hasAudio()? "YES" : "no",
		hasRadio()? "YES" : "no",
		hasAsyncIo()? "YES" : "no",
		hasStreaming()? "YES" : "no"
	);
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

bool Capability::hasVideoOutput()
{
	doQuery();
	return bool(capability.capabilities & V4L2_CAP_VIDEO_OUTPUT);
}

bool Capability::hasVideoOverlay()
{
	doQuery();
	return bool(capability.capabilities & V4L2_CAP_VIDEO_OVERLAY);
}

bool Capability::hasVbiCapture()
{
	doQuery();
	return bool(capability.capabilities & V4L2_CAP_VBI_CAPTURE);
}

bool Capability::hasVbiOutput()
{
	doQuery();
	return bool(capability.capabilities & V4L2_CAP_VBI_OUTPUT);
}

bool Capability::hasSlicedVbiCapture()
{
	doQuery();
	return bool(capability.capabilities & V4L2_CAP_SLICED_VBI_CAPTURE);
}

bool Capability::hasSlicedVbiOutput()
{
	doQuery();
	return bool(capability.capabilities & V4L2_CAP_SLICED_VBI_OUTPUT);
}

bool Capability::hasRdsCapture()
{
	doQuery();
	return bool(capability.capabilities & V4L2_CAP_RDS_CAPTURE);
}

bool Capability::hasVideoOutputOverlay()
{
	doQuery();
	return bool(capability.capabilities & V4L2_CAP_VIDEO_OUTPUT_OVERLAY);
}

bool Capability::hasTuner()
{
	doQuery();
	return bool(capability.capabilities & V4L2_CAP_TUNER);
}

bool Capability::hasAudio()
{
	doQuery();
	return bool(capability.capabilities & V4L2_CAP_AUDIO);
}

bool Capability::hasRadio()
{
	doQuery();
	return bool(capability.capabilities & V4L2_CAP_RADIO);
}

bool Capability::hasReadWrite()
{
	doQuery();
	return bool(capability.capabilities & V4L2_CAP_READWRITE);
}

bool Capability::hasAsyncIo()
{
	doQuery();
	return bool(capability.capabilities & V4L2_CAP_ASYNCIO);
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
	printf("Querying camera capability...\n");
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
