#ifndef V4L2_Camera
#define V4L2_Camera

/**
 * Camera is an extension of Device, providing a better API, albeit a bit
 * of an assumption on what those who use V4L2 want to accomplish. 
 */

#include "../wrap/Device.hpp"

namespace V4L2 {
	class Buffers;
	class Frame;
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

		/**
		 * Get the next frame.
		 * If the device isn't streaming (or open), turn it on. 
		 */
		Frame* grabFrame();

		/**
		 * TODO: DOCS, basically these pass through
		 */
		bool dequeue();
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
