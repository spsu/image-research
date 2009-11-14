#ifndef V4L2_Buffer
#define V4L2_Buffer

#include <linux/videodev2.h>

namespace V4L2 {
class Buffer
{
	public:
		Buffer();

		~Buffer();

		void reset();

		v4l2_buffer* 

	protected:
		/**
		 * V4L2 Buffer structure.
		 */
		struct v4l2_buffer buffer;
};
}

#endif
