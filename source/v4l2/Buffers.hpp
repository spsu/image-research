#ifndef V4L2_Buffers
#define V4L2_Buffers

#include <vector>

/**
 *		TODO
 *		TODO: RENAME TO SOMETHING OTHER THAN "Buffers",
 *					Perhaps BufferSet? BufferManagement, blah... 
 */

namespace V4L2 {
	class Buffer;
	class RequestBuffers;
	class Device;
}

namespace V4L2 {
class Buffers 
{
	public:
		Buffers(Device* dev);

		~Buffers();

		/**
		 * Get the request buffers object.
		 * Caller does not own the reference. 
		 */
		RequestBuffers* getRequest();

		/**
		 * Initialize buffers.
		 */
		bool initBuffers();

		/**
		 * Get the buffer vector.
		 */
		std::vector<Buffer*>* getBuffers() { return buffers; };

		/**
		 * Get a single buffer.
		 */
		Buffer* getBuffer(int offset);

	private:
		Device* device;

		RequestBuffers* reqbuf;

		std::vector<Buffer*>* buffers;

};
}

#endif
