#ifndef V4L2_Buffers
#define V4L2_Buffers

#include <vector>

/**
 * Vector of buffers, necessary to queue/dequeue from. 
 * XXX: This shouldn't be a part of the public API. 
 * TODO: "Buffers" is a bad class name wrt "Buffer" and "RequestBuffer"
 * TODO: Rename BufferManagement or BufferManager
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

		/**
		 * Get the device pointer
		 */
		Device* getDevice() { return device; };

		// ==================== NEW API STUFF ==================== //

		/**
		 * Dequeues a buffer from the device driver.
		 * Can't exceed the number of buffers allocated. 
		 */
		bool dequeue(bool manual = true);

		/**
		 * Dequeues a buffer from the device driver, but only if there are no
		 * outstanding buffers already dequeued. This ensures only one buffer
		 * is checked out at a time if this method is used exclusively.
		 */
		bool dequeueOne(bool manual = true);

		/**
		 * Returns a buffer to the device driver.
		 */
		bool queue();

	protected:
		/**
		 * Shared pointer to the device class.
		 * This allows access to the device.
		 */
		Device* device;

		/**
		 * Shared pointer(?) to RequestBuffers structure, which sets up driver
		 * buffers. This tells us how many of our requested buffers were granted
		 * and generally what type of buffers we have. 
		 */
		RequestBuffers* reqbuf;

		/**
		 * The buffers being managed by this class.
		 * TODO: Verify this is necessary. 
		 */
		std::vector<Buffer*>* buffers;

		/**
		 * TODO: Verify this is necessary.
		 */
		Frame* lastFrame;

		/**
		 * Number of currently dequeued buffers (for the application to use).
		 * This must be compared with the RequestBuffers structure to ensure we
		 * don't hit the limit/need to wait. 
		 */
		int numDequeued;

		/**
		 * If the *user* just manually dequeued a buffer, this is set to true.
		 * When grabFrame() is called, if this is false the method must assume
		 * a buffer needs to be automatically dequeued. THIS IS SLOW!!! It's 
		 * far better for the user to manually queue/dequeue to ensure the 
		 * pipeline is as close to realtime as possible.
		 */
		 bool manuallyDequeued;
};
}

#endif
