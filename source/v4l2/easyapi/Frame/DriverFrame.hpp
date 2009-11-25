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
		 * XXX: Still needs refactoring!
		 */
		DriverFrame(Buffers* bufs, Buffer* curBuf, bool doQueue);

		/**
		 * DTOR.
		 */
		virtual ~DriverFrame();

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
		 * TODO: This whole design is really ugly...
		 */
		Buffer* curBuffer;

		/**
		 * Pointer to the allocated buffers.
		 */
		Buffers* buffers;

		/**
		 * Whether the frame should auto-queue the buffer back to the driver on
		 * object deletion. (TODO: Defaults to true. Is there another way to do
		 * this sensibly?)
		 */
		bool doAutoQueue;

};
}

#endif
