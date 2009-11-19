#include "Frame.hpp"
#include "Buffers.hpp"
#include "Camera.hpp"
#include "../wrap/Buffer.hpp"
#include "../wrap/Device.hpp"
#include "../wrap/Format.hpp"
#include <stdio.h>

namespace V4L2 {

Frame::Frame(Buffers* bufs)
{
	buffers = bufs;
	curBuffer = new Buffer(buffers->getRequest());

	// Dequeue to process
	if(!curBuffer->dequeue(buffers->getDevice())) {
		fprintf(stderr, "Could not dequeue buffer\n");
	}
}

Frame::~Frame()
{
	// Queue it back at the camera 
	if(!curBuffer->queue(buffers->getDevice())) {
		fprintf(stderr, "Could not queue buffer\n");
	}
	delete curBuffer;
}

unsigned char* Frame::getData()
{
	return (unsigned char*)buffers->getBuffer(curBuffer->getIndex())->getStart();
}

int Frame::getBytesUsed()
{
	return curBuffer->getBytesUsed();
}

int Frame::getWidth()
{
	Camera* cam = 0;
	cam = (Camera*)buffers->getDevice();
	return cam->getFormat()->getWidth();
}

int Frame::getHeight()
{
	Camera* cam = 0;
	cam = (Camera*)buffers->getDevice();
	return cam->getFormat()->getHeight();
}


} // end namespace V4L2
