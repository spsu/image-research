#include "Buffer.hpp"
#include "RequestBuffers.hpp"
#include "Device.hpp"
#include <stdio.h>
#include <string.h> // memset
#include <stropts.h> // ioctl
#include <errno.h>

namespace V4L2 {

Buffer::Buffer()
{
	reset();
}

Buffer::Buffer(RequestBuffers* reqbuf, int index)
{
	reset();
	buffer.type = reqbuf->getType();
	buffer.memory = reqbuf->getMemory();
	buffer.index = index;
}

Buffer::~Buffer()
{
}

void Buffer::reset()
{
	memset(&buffer, 0, sizeof(buffer));
}

bool Buffer::query(Device* dev)
{
	if(ioctl(dev->fd, VIDIOC_QUERYBUF, &buffer) == -1) {
		fprintf(stderr, "Querying the buffer failed\n");
		return false;
	}
	return true;
}

bool Buffer::queue(Device* dev)
{
	if(ioctl(dev->fd, VIDIOC_QBUF, &buffer) == -1) {
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
	if(ioctl(dev->fd, VIDIOC_DQBUF, &buffer) == -1) {
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

} // end namespace V4L2
