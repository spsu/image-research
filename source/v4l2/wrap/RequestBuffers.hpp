#ifndef V4L2_RequestBuffers
#define V4L2_RequestBuffers

/**
 * Request Buffers from the device driver.
 * Wraps the v4l2_requestbuffers structure, as well as provides additional 
 * functionality. 
 */

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
		/**
		 * CTOR.
		 */
		RequestBuffers(Device* dev = NULL);

		/**
		 * DTOR.
		 */
		~RequestBuffers();

		/**
		 * Reset the request buffers structure.
		 */
		void reset();

		/**
		 * Make the buffer request to the device driver.
		 */
		bool makeRequest(Device* dev = NULL);

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

		/**
		 * Temporary pointer to the device.
		 * TODO: Clean this up?
		 */
		Device* device;
};
}

#endif
