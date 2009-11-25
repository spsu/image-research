#ifndef V4L2_Buffer
#define V4L2_Buffer

#include <linux/videodev2.h>
#include <stdlib.h> // size_t

namespace V4L2 {

class RequestBuffers;
class Device;

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
		// NOT IMPLEMENTING UNTIL/UNLESS IT IS VITAL
		// Cleanup
		//for (i = 0; i < reqbuf.count; i++)
		//	munmap (buffers[i].start, buffers[i].length);

		// ==================== MAPPED MEMORY ==================== //

		/**
		 * Perform the map operation.
		 */
		bool map(Device* dev);

		/**
		 * Get the start of mapped memory.
		 */
		void* getStart() { return start; };

		/**
		 * Get the length of the mapped memory.
		 */
		//size_t getLength() { return length; };



	protected:
		/**
		 * V4L2 Buffer structure.
		 */
		struct v4l2_buffer buffer;

		/**
		 * User-space mapped memory start position.
		 * (Only for memory mapped buffers.)
		 */
		void* start;

		/**
		 * Length of the mapped memory buffer.
		 */
		//size_t length;

		/**
		 * Whether or not memory has been mapped.
		 */
		bool mapped;
};

} // end namespace V4L2

#endif
