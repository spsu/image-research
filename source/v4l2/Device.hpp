#ifndef V4L2_Device
#define V4L2_Device

#include <string>

namespace V4L2 {
	class Capability;
	class Format;
}

/**
 * Represents a Video4Linux2 interface to a hardware camera.
 */
namespace V4L2 {
class Device 
{
	friend class Buffer;
	friend class Capability;
	friend class Format;
	friend class RequestBuffers;

	public:
		/**
		 * CTOR.
		 */
		Device(const std::string& fName);

		/**
		 * DTOR.
		 */
		~Device();

		/**
		 * Open the device. Returns true if successful.
		 */
		bool open();

		/**
		 * If the device is open.
		 */
		bool isOpen();

		/**
		 * Close the device.
		 */
		void close();

		// XXX XXX TEMPORARY!
		int getFd();

		/**
		 * Turn the stream on.
		 */
		bool streamOn();

		/**
		 * Turn the stream off.
		 */
		bool streamOff();

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

	private:
		/**
		 * Name
		 */
		std::string name;

		/**
		 * File descriptor.
		 */
		int fd;

		/**
		 * Capabilities of the device
		 */
		Capability* capability;

		/**
		 * Format of the device
		 */
		Format* format;
};
}

#endif
