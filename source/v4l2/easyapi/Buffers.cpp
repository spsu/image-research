#include "Buffers.hpp"
#include "Frame.hpp"
#include "../wrap/Buffer.hpp"
#include "../wrap/RequestBuffers.hpp"
#include <stdio.h>

namespace V4L2 {

Buffers::Buffers(Device* dev):
	reqbuf(NULL),
	buffers(NULL),
	lastFrame(NULL)
{
	device = dev;
	reqbuf = new V4L2::RequestBuffers(dev);
	buffers = new std::vector<Buffer*>();
}

Buffers::~Buffers()
{
	// TODO: DELETE BUFFERS
	delete reqbuf;
}

RequestBuffers* Buffers::getRequest()
{
	return reqbuf;
}

bool Buffers::initBuffers()
{
	if(!reqbuf->makeRequest()) {
		fprintf(stderr, "Buffers::initBuffers() error with making request.\n");
		return false;
	}

	printf("\tNumber of buffers allocated: %d\n", reqbuf->getCount());

	// Query, queue, and *map* the buffers.
	// TODO XXX: If an error occurs in the process, I need to clean it up
	for(int i = 0; i < reqbuf->getCount(); i++) {

		V4L2::Buffer* buffer = new V4L2::Buffer(reqbuf, i);

		if(!buffer->query(device)) {
			fprintf(stderr, "Buffer couldn't be queried.\n");
			return false;
		}

		if(!buffer->queue(device)) {
			return false;
		}

		if(!buffer->map(device)) {
			fprintf(stderr, "Memory map failed!\n");
			return false;
		}

		buffers->push_back(buffer);
	}
	return true;
}

Buffer* Buffers::getBuffer(int offset) 
{
	// TODO: Bounds checking
	return buffers->at(offset);
}

Frame* Buffers::grabFrame()
{
	if(lastFrame != NULL) {
		delete lastFrame;
	}
	lastFrame = new Frame(this);
	return lastFrame;
}

} // end namespace V4L2
