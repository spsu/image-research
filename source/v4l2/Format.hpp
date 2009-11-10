#ifndef V4L2_Format
#define V4L2_Format

// XXX XXX: This might need to be refactored significantly when 
// XXX XXX: Set State implemented

#include <linux/videodev2.h> // v4l2_capability struct

namespace V4L2 {
	class Device;
}

namespace V4L2 {
class Format
{
	public:
		Format();

		Format(Device* dev);

		~Format();

		int getWidth();
		int getHeight();

	private:
		/**
		 * V4L2 format structure.
		 */
		struct v4l2_format format;

		bool queried;

		/**
		 * The device this format was fetched from.
		 */
		Device* device;

		bool doQuery();

};
}

#endif
