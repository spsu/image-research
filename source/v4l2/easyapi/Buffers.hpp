#ifndef V4L2_Buffers
#define V4L2_Buffers

#include <vector>

/**
 * Vector of buffers, necessary to queue/dequeue from. 
 * XXX: This shouldn't be a part of the public API. 
 * TODO: "Buffers" is a bad class name wrt "Buffer" and "RequestBuffer"
 */

namespace V4L2 {
	class Buffer;
	class RequestBuffers;
	class Device;
	class Frame;
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

		/**
		 * Get a frame in one shot. (Queue->Grab->Dequeue)
		 * This is great for one camera, but for 2+ cameras where timing is 
		 * important, this will result in poor timing between the frames. 
		 */
		Frame* grabFrame();

		bool queueFrame();
		Frame* getFrame();
		bool dequeueFrame();

		/**
		 * Get the device pointer
		 */
		Device* getDevice() { return device; };

	private:
		Device* device;

		RequestBuffers* reqbuf;

		std::vector<Buffer*>* buffers;

		Frame* lastFrame;

};
}

#endif
