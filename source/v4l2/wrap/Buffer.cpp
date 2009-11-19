#include "Buffer.hpp"
#include "RequestBuffers.hpp"
#include "Device.hpp"
#include <stdio.h>
#include <string.h> // memset
#include <sys/mman.h> // mmap
#include <stropts.h> // ioctl
#include <errno.h>

namespace V4L2 {

Buffer::Buffer():
	start(NULL),
	mapped(false)
{
	reset();
}

Buffer::Buffer(RequestBuffers* reqbuf, int index):
	start(NULL),
	mapped(false)
{
	reset();
	buffer.type = reqbuf->getType();
	buffer.memory = reqbuf->getMemory();
	buffer.index = index;
}

Buffer::~Buffer()
{
	if(mapped) {
		// TODO: Unmap and free!
	}
}

void Buffer::reset()
{
	if(mapped) {
		fprintf(stderr,
			"Buffer::reset() cannot reset struct while memory is mapped\n");
		return;
	}
	memset(&buffer, 0, sizeof(buffer));
}

bool Buffer::query(Device* dev)
{
	if(ioctl(dev->getFd(), VIDIOC_QUERYBUF, &buffer) == -1) {
		fprintf(stderr, "Querying the buffer failed\n");
		return false;
	}
	return true;
}

bool Buffer::queue(Device* dev)
{
	if(ioctl(dev->getFd(), VIDIOC_QBUF, &buffer) == -1) {
		switch(errno) {
			case EAGAIN:
				fprintf(stderr, "EAGAIN\n");
				break;
			case EINVAL:
				fprintf(stderr, "EINVAL\n");
				break;
			case ENOMEM:
				fprintf(stderr, "ENOMEM\n");
				break;
			case EIO:
				fprintf(stderr, "EIO\n");
				break;
			default:
				fprintf(stderr, "There was an error queueing\n");
		}
		return false;
	}
	return true;
}

bool Buffer::dequeue(Device* dev)
{
	if(ioctl(dev->getFd(), VIDIOC_DQBUF, &buffer) == -1) {
		switch(errno) {
			case EAGAIN:
				fprintf(stderr, "EAGAIN\n");
				break;
			case EINVAL:
				fprintf(stderr, "EINVAL\n");
				break;
			case ENOMEM:
				fprintf(stderr, "ENOMEM\n");
				break;
			case EIO:
				fprintf(stderr, "EIO\n");
				break;
			default:
				fprintf(stderr, "There was an error dequeueing\n");
		}
		return false;
	}
	return true;
}

int Buffer::getLength()
{
	return (int)buffer.length;
}

int Buffer::getOffset()
{
	return (int)buffer.m.offset;
}

int Buffer::getIndex()
{
	return (int)buffer.index;
}

int Buffer::getBytesUsed()
{
	return (int)buffer.bytesused;
}

bool Buffer::map(Device* dev)
{
	if(mapped) {
		fprintf(stderr, "Buffer::map() cannot map memory again!\n");
		return false;
	}

	if(dev == NULL) {
		fprintf(stderr, "Buffer::map() invalid device supplied.\n");
		return false;
	}

	start = mmap(NULL, 
				 getLength(),
				 PROT_READ | PROT_WRITE, // recommended
				 MAP_SHARED,             // recommended
				 dev->getFd(), 
				 getOffset()
	);

	if(start == MAP_FAILED) {
		// TODO: If failure, I need to unmap() and free() the buffers already
		// mapped prior to this one...
		fprintf(stderr, "Buffer::map() err: Could not map memory!\n");
		return false;
	}

	mapped = true;
	return true;
}


} // end namespace V4L2
