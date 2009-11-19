#include "Frame.hpp"
#include "Buffers.hpp"
#include "../wrap/Buffer.hpp"
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

int Frame::getLength()
{
	return curBuffer->getBytesUsed();
}


} // end namespace V4L2
