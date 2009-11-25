#include "Buffers.hpp"
#include "Frame.hpp"
#include "../wrap/Buffer.hpp"
#include "../wrap/RequestBuffers.hpp"
#include <stdio.h>

namespace V4L2 {

Buffers::Buffers(Device* dev):
	reqbuf(NULL),
	buffers(NULL),
	lastFrame(NULL),
	numDequeued(0),
	manuallyDequeued(false)
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

Frame* Buffers::grabFrame2()
{
	// If a buffer wasn't manually dequeued by the user prior to calling, we 
	// need to dequeue a buffer now. 
	if(!manuallyDequeued && !dequeue(false)) {
		fprintf(stderr,
			"Buffers::grabFrame() a buffer could not be auto dequeued " \
			"to grab a frame.\nPerhaps all buffers are already dequeued.\n");
		return 0;
	}
	manuallyDequeued = false;

	// TODO: DO GRAB FRAME.
	// XXX: If the frame was auto-dequeued, we need to tell it to auto-queue on
	// delete.

}

bool Buffers::dequeue(bool manual)
{
	int count = 0;

	count = reqbuf->getCount();
	if(count < 1 ) {
		fprintf(stderr, "Buffers::dequeue() no buffers available.\n");
		return false;
	}
	if(numDequeued >= count) {
		fprintf(stderr, 
			"Buffers::dequeue() too many buffers are dequeued: %d of %d.\n",
			numDequeued, count);
		return false;
	}

	// TODO: Buffer class needs refactor, so thus does the following:
	if(!buffers->at(0)->dequeue(device)) {
		return false;
	}
	numDequeued++;
	manuallyDequeued = manual;
	return true;
}

bool Buffers::dequeueOne(bool manual)
{
	if(numDequeued >= 1) {
		fprintf(stderr, 
			"Buffers::dequeueOne() can only dequeue one at a time\n");
		return false;
	}
	return dequeue(manual);
}

bool Buffers::queue()
{
	if(numDequeued < 1) {
		fprintf(stderr, 
			"Buffers::queue() there is nothing to return to queue\n");
		return false;
	}
	if(!buffers->at(0)->queue(device)) {
		return false;
	}
	numDequeued--;
	return true;
}

} // end namespace V4L2
