#ifndef V4L2_Camera
#define V4L2_Camera

/**
 * Camera is an extension of Device, providing a better API, albeit a bit
 * of an assumption on what those who use V4L2 want to accomplish. 
 */

#include "../wrap/Device.hpp"

namespace V4L2 {
	class Buffers;
	class DriverFrame;
	//class BufferFrame;
}

namespace V4L2 {
class Camera : public Device
{
	public: 
		/**
		 * Camera CTOR.
		 * Provide the path to the device.
		 */
		Camera(const std::string& devicePath);

		/**
		 * Camera DTOR.
		 */
		~Camera();

		/**
		 * Print camera information. 
		 * Includes capabilities, format, etc.
		 */
		void printInfo();

		/**
		 * Get the camera capability object.
		 * Caller does not own the object.
		 */
		Capability* getCapability();

		/**
		 * Get the camera image format object.
		 * Caller does not own the object, but may use it to set format options.
		 */
		Format* getFormat();

		/**
		 * Set the camera image format.
		 * This destroys the old format object, and the caller no longer owns 
		 * the passed object.
		 */
		//void setFormat(Format* fmt);

		void setupBuffers();
		Buffers* getBuffers(){ return buffers; }; // TODO: Is this necessary?


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


		// ============== MULTI-CAMERA SYNCHRONIZATION ====================== //

		/**
		 * Manual dequeue of a buffer from the driver.
		 * This should aid with multi-device synchronization purposes if it's 
		 * called together with the other camera. 
		 */
		bool dequeue();

		/**
		 * Manual dequeue of ONE AND ONLY ONE buffer from the driver.
		 * (Cannot be called twice before requeue takes place, or it fails.)
		 * This should aid with multi-device synchronization purposes if it's 
		 * called together with the other camera. 
		 */
		bool dequeueOne();

	protected:
		/**
		 * Capabilities of the device
		 */
		Capability* capability;

		/**
		 * Format of the device
		 */
		Format* format;

		/**
		 * Buffers.
		 */
		Buffers* buffers;

};
}

#endif
