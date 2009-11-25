#ifndef V4L2_Buffers
#define V4L2_Buffers

#include <vector>
#include <queue>

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
	class DriverFrame;
}

namespace V4L2 {
class Buffers 
{
	public:
		/**
		 * CTOR.
		 */
		Buffers(Device* dev);

		/**
		 * DTOR.
		 */
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
		 * Get the device pointer
		 */
		Device* getDevice() { return device; };


		// ===================== IMAGE CAPTURE ============================== //

		/**
		 * Grab a temporary frame (a buffer wrapper object) from the driver.
		 * Temporary frames are deleted and have their buffers requeued
		 * automatically when this method is called again.
		 *
		 * DO NOT MANUALLY DEALLOCATE THE DRIVERFRAME!
		 *
		 * Note: If the device isn't already streaming (or open), this turns it
		 * on as a matter of convenience.
		 *
		 * Also note that if the frame wasn't manually dequeued prior to grab,
		 * this will dequeue and grab in one shot (which should result in poor
		 * timing with dual [or more] synchronous cameras.)
		 */ 
		DriverFrame* grabTemporaryFrame();

		/**
		 * Grab a buffered frame from the driver, saving its memory into the
		 * object and allowing many frames to be captured and reused. This 
		 * requires the caller to delete the frame when they are done. 
		 *
		 * Note: If the device isn't already streaming (or open), this turns it
		 * on as a matter of convenience.
		 *
		 * Also note that if the frame wasn't manually dequeued prior to grab,
		 * this will dequeue and grab in one shot (which should result in poor
		 * timing with dual [or more] synchronous cameras.)
		 */ 
		//BufferFrame* grabPersistentFrame(); // XXX TODO (I don't need it now)


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
		 * XXX: Currently doesn't work since Frames are deallocated in grabFrame
		 * method(s). It's impossible to manually requeue before dequeuing 
		 * currently.
		 */
		bool dequeueOne(bool manual = true);

		/**
		 * Returns a buffer to the device driver.
		 */
		//bool queue(); // TODO: Not sure how this would work just yet

		/**
		 * Report that a buffer has been requeued by another object.
		 */
		void reportQueued();

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
		 * Used for auto-deallocation with the grabTemporaryFrame() method. 
		 */
		DriverFrame* lastFrame;

		/**
		 * Number of currently dequeued buffers (for the application to use).
		 * This must be compared with the RequestBuffers structure to ensure we
		 * don't hit the limit/need to wait. 
		 */
		int numDequeued;

		/**
		 * Every time the user manually dequeues, the buffer is stored here 
		 * until it is used (eg. from grabTemporaryFrame() or related.)
		 */
		std::queue<Buffer*> manuallyDequeued;
};
}

#endif
