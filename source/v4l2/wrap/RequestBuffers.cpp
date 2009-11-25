#include "RequestBuffers.hpp"
#include "Device.hpp"
#include <stropts.h> // ioctl
#include <stdio.h>
#include <string.h> // memset
#include <errno.h>

namespace V4L2 {

RequestBuffers::RequestBuffers(Device* dev)
{
	device = dev;

	reset();
	reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	reqbuf.memory = V4L2_MEMORY_MMAP; // or V4L2_MEMORY_USERPTR
	// video output requires at least two buffers, one displayed and one filled 
	// by the application.
	reqbuf.count = 2; // ONLY FOR V4L2_MEMORY_MMAP
}

RequestBuffers::~RequestBuffers()
{

}

void RequestBuffers::reset()
{
	memset(&reqbuf, 0, sizeof(reqbuf));
}

bool RequestBuffers::makeRequest(Device* dev)
{
	if(device == NULL && dev == NULL) {
		fprintf(stderr, "There is no device to request buffers from.\n");
		return false;
	}
	if(dev == NULL) {
		dev = device;
	}

	if(ioctl(dev->getFd(), VIDIOC_REQBUFS, &reqbuf) == -1) {
		switch(errno) {
			case EBUSY:
				fprintf(stderr, 
					"Buffer busy err: IO in progress, or mem still mapped.\n");
				break;
			case EINVAL:
				fprintf(stderr, 
					"Buffer type or IO method unsupported.\n");
				break;
			default:
				fprintf(stderr, 
					"An unknown request buffer error occurred.\n");
		}
		return false;
	}
	return true;
}

int RequestBuffers::getCount()
{
	return reqbuf.count;
}

v4l2_buf_type RequestBuffers::getType()
{
	return reqbuf.type;
}

v4l2_memory RequestBuffers::getMemory()
{
	return reqbuf.memory;
}

} // end namespace V4L2
