#ifndef V4L2_Frame
#define V4L2_Frame

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

		int getLength();

	protected:
		Buffer* curBuffer;
		Buffers* buffers;
};
}

#endif
