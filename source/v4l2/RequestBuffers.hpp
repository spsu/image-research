#ifndef V4L2_RequestBuffers
#define V4L2_RequestBuffers

#include <linux/videodev2.h>
namespace V4L2 {
	class Buffer;
	class Device;
}

namespace V4L2 {
class RequestBuffers
{
	friend class Buffer;

	public:
		RequestBuffers(Device* dev = NULL);

		~RequestBuffers();

		void reset();

		/**
		 * Make the request
		 */
		bool makeRequest(Device* dev = NULL);

		//v4l2_requestbuffers* getPtr() { return &reqbuf; }

		/**
		 * Only applies to memory mapped.
		 */
		int getCount();

		v4l2_buf_type getType();
		v4l2_memory getMemory();

	protected:
		/**
		 * V4L2 Request Buffers structure.
		 */
		struct v4l2_requestbuffers reqbuf;

		Device* device;

};
}

#endif
