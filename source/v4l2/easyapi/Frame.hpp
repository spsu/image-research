#ifndef V4L2_Frame
#define V4L2_Frame

/**
 * Provides easy (relatively abstract) access to the memory mapped buffer.
 * INTERFACE.
 */
namespace V4L2 {
	class Buffer;
	class Buffers;
}

namespace V4L2 {
class Frame
{
	public:

		/**
		 * Get the starting pointer where data is located.
		 * You'll have to call getBytesUsed() to read the exact amount.
		 */
		virtual unsigned char* getData() = 0;

		/**
		 * Get the number of bytes used to represent the image frame.
		 */
		virtual int getBytesUsed() = 0;

		/**
		 * Get the width of the image.
		 */
		virtual int getWidth() = 0;

		/**
		 * Get the height of the image.
		 */
		virtual int getHeight() = 0;

};
}

#endif
