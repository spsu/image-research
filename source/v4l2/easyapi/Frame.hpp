#ifndef V4L2_Frame
#define V4L2_Frame

/**
 * Provides easy access to the memory mapped segment.
 * TODO: This code, as well as the code in "Buffers", is *really* bad.
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
		 * Wraps the current buffer. 
		 * Very slow for dual camera use.
		 */
		Frame(Buffers* bufs); // TODO: DEPRECATE

		Frame(Buffers* bufs, Buffer* curBuf, bool doQueue);

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
		Buffer* curBuffer;
		Buffers* buffers;

		/**
		 * Whether the object should requeue the buffer when DTOR called. 
		 * (Should be true if the frame was automatically dequeued, false if
		 * manually dequeued.)
		 */
		bool doAutoQueue;

};
}

#endif
