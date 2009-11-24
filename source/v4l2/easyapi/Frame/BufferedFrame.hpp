#ifndef V4L2_Frame_BufferedFrame
#define V4L2_Frame_BufferedFrame

/**
 * Buffered Frame
 * This class copies the image present in device driver memory to an in-object
 * buffer. The class will not require access to the device driver after 
 * instantiation. 
 */

#include "../Frame.hpp"

namespace V4L2 {
class BufferedFrame: public Frame
{
	public:
		/**
		 * Wraps the current buffer. 
		 * Very slow for dual camera use.
		 */
		Frame(Buffers* bufs);

		/**
		 * DTOR.
		 * Clears buffer memory. 
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
		 * Image data.
		 */
		unsigned char* data;

		/**
		 * Image length.
		 */
		int numBytes; 

};
}

#endif
