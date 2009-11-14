#ifndef V4L2_Buffer
#define V4L2_Buffer

#include <linux/videodev2.h>
#include <stdlib.h> // size_t

namespace V4L2 {

class RequestBuffers;
class Device;

enum queueState { ENQUEUED, DEQUEUED, NONE };

struct bbuffer {
	void* start;
	size_t length;
	queueState state;
};

/**
 * XXX FIXME: Obviously in need of a major refactor.
 */
class Buffer
{
	public:
		Buffer();
		Buffer(RequestBuffers* reqbuf, int index = 0);

		~Buffer();

		void reset();

		bool query(Device* dev);
		bool queue(Device* dev);
		bool dequeue(Device* dev);

		int getLength();
		int getOffset();
		int getIndex();
		int getBytesUsed();

		// XXX: NOTE, NEED CLEANUP MECHANISM:
		// Cleanup
		//for (i = 0; i < reqbuf.count; i++)
		//	munmap (buffers[i].start, buffers[i].length);

	protected:
		/**
		 * V4L2 Buffer structure.
		 */
		struct v4l2_buffer buffer;
};

} // end namespace V4L2

#endif
