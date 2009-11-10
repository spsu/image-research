#ifndef V4L2_Device
#define V4L2_Device

#include <string>

namespace V4L2 {
	class Capabilities;
	class Format;
}

/**
 * Represents a Video4Linux2 interface to a hardware camera.
 */
namespace V4L2 {
class Device 
{
	friend class Capabilities;
	friend class Format;

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
		 * Get the camera capabilities object.
		 * Caller does not own the object.
		 */
		Capabilities* getCapabilities();

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
		 * Capabilities of the device (cached)
		 */
		Capabilities* cap;
};
}

#endif
