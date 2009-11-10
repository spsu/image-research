#ifndef V4L2_Device
#define V4L2_Device

#include <string>

/**
 * Represents a Video4Linux2 interface to a hardware camera.
 */
namespace V4L2 {
class Device 
{
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
		int tempGetHandle() { return handle; }

	private:
		/**
		 * Name
		 */
		std::string name;

		/**
		 * File handle.
		 */
		int handle;
};
}

#endif
