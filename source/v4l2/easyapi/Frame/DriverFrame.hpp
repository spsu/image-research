#ifndef V4L2_Frame_DriverFrame
#define V4L2_Frame_DriverFrame

#include "../Frame.hpp"

namespace V4L2 {
	class Buffers;
	class Buffer;
}

namespace V4L2 {
class DriverFrame: public Frame
{
	public:
		/**
		 * Wraps the current buffer. 
		 * Very slow for dual camera use.
		 */
		Frame(Buffers* bufs);

		/**
		 * DTOR.
		 */
		virtual ~Frame();

		/**
		 * Get the starting pointer where data is located.
		 * You'll have to call getBytesUsed() to read the exact amount.
		 */
		virtual unsigned char* getData();

		/**
		 * Get the number of bytes used to represent the image frame.
		 */
		virtual int getBytesUsed();

		/**
		 * Get the width of the image.
		 */
		virtual int getWidth();

		/**
		 * Get the height of the image.
		 */
		virtual int getHeight();

	protected:
		/**
		 * The current buffer representation.
		 * TODO: This is really ugly...
		 */
		Buffer* curBuf;

		/**
		 * Pointer to the allocated buffers.
		 */
		Buffers* buffers;

};
}

#endif
