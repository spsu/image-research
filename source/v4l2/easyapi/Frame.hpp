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
		Frame(Buffers* bufs);

		~Frame();

		unsigned char* getData();

		int getBytesUsed();
		int getWidth();
		int getHeight();

	protected:
		Buffer* curBuffer;
		Buffers* buffers;
};
}

#endif
