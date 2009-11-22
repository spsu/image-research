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
		 * Maps the device with the given path. 
		 */
		Device(const std::string& fName);

		/**
		 * DTOR.
		 */
		virtual ~Device();

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

		/**
		 * Get the file descriptor.
		 * Automatically opens the device if it is not already open. 
		 */
		int getFd();

		/**
		 * Turn the stream on.
		 * Automatically opens the device if it is not already open. 
		 */
		bool streamOn();

		/**
		 * Turn the stream off.
		 */
		bool streamOff();

		/**
		 * Whether the device is streaming.
		 */
		bool isStreaming() { return streaming; };

	protected:
		/**
		 * Filename/path
		 */
		std::string name;

		/**
		 * File descriptor of an open device.
		 */
		int fd;

		/**
		 * Device is streaming.
		 */
		bool streaming;
};
}

#endif
