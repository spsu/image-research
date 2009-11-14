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
 * Represents a V4L2 device format, and can be used to get/set/try various
 * format options.
 * Does not need to be associated with a single Device, so it may be used and
 * reused between multiple devices.
 */
namespace V4L2 {
class Format
{
	public:
		Format();

		/**
		 * XXX: Does this mean it is tied to the device??
		 */
		Format(Device* dev);

		~Format();

		/**
		 * Reset the structure to default.
		 * XXX: Should I have this?? Shouldn't I just make a new Format struct?
		 */
		void reset(); // rename 'clear()' ?

		bool getFormat(Device* dev = NULL, bool doReset = true);
		bool setFormat(Device* dev = NULL);
		bool tryFormat(Device* dev = NULL);

		int getWidth();
		int getHeight();
		int getBytesPerLine();

		/**
		 * Attempt at mutators. Maybe refactor?
		 */
		void setWidth(int w);
		void setHeight(int h);

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

	protected:
		/**
		 * V4L2 format structure.
		 */
		struct v4l2_format format;

		/**
		 * Whether the format has been queried (since the last reset).
		 */
		bool queried;

		/**
		 * Perform the 'get', 'set', or 'try' format query.
		 */
		bool query(int fd, int request);

	private:

		/**
		 * The device this format was fetched from.
		 */
		Device* device;

		bool doQuery(); // XXX: Remove

};
}

#endif
