#ifndef V4L2_Format
#define V4L2_Format

// XXX XXX: This might need to be refactored significantly when 
// XXX XXX: Set State implemented

#include <linux/videodev2.h>

struct v4l2_format;

namespace V4L2 {
	class Device;
}

/**
 * Represents camera format.
 * Get or set these.
 */
namespace V4L2 {
class Format
{
	public:
		Format();

		Format(Device* dev);

		~Format();

		// Reset the struct to default/empty values
		void resetStruct();

		int getWidth();
		int getHeight();
		int getBytesPerLine();

		/**
		 * Get the number of bytes the image uses.
		 */
		int getImageSize();

		const char* getPixelFormat();
		int getPixelFormatCode();

		const char* getColorspace();
		int getColorspaceCode();

		const char* getField();
		int getFieldCode();

	private:
		/**
		 * V4L2 format structure.
		 */
		struct v4l2_format format;

		/**
		 * Whether the format has been queried.
		 */
		bool queried;

		/**
		 * The device this format was fetched from.
		 */
		Device* device;

		bool doQuery();

};
}

#endif
