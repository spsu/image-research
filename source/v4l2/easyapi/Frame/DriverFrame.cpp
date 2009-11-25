#include "DriverFrame.hpp"
#include "../Buffers.hpp"
#include "../Camera.hpp"
#include "../../wrap/Buffer.hpp"
#include "../../wrap/Device.hpp"
#include "../../wrap/Format.hpp"
#include <stdio.h>

namespace V4L2 {

DriverFrame::DriverFrame(Buffers* bufs, Buffer* curBuf, bool doQueue)
{
	buffers = bufs;
	curBuffer = curBuf;
	//doAutoQueue = doQueue;
	doAutoQueue = true; // XXX: TEMP. All Requeues in DTOR for now
}

DriverFrame::~DriverFrame()
{
	if(doAutoQueue) {
		// Queue it back at the camera 
		if(!curBuffer->queue(buffers->getDevice())) {
			fprintf(stderr, "Could not queue buffer\n");
		}
		delete curBuffer;
		buffers->reportQueued();
	}
}

unsigned char* DriverFrame::getData()
{
	return (unsigned char*)
				buffers->getBuffer(curBuffer->getIndex())->getStart();
}

int DriverFrame::getBytesUsed()
{
	return curBuffer->getBytesUsed();
}

int DriverFrame::getWidth()
{
	Camera* cam = 0;
	cam = (Camera*)buffers->getDevice();
	return cam->getFormat()->getWidth();
}

int DriverFrame::getHeight()
{
	Camera* cam = 0;
	cam = (Camera*)buffers->getDevice();
	return cam->getFormat()->getHeight();
}

} // end namespace V4L2
