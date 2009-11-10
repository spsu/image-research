#ifndef V4L2_Capabilities
#define V4L2_Capabilities

#include <string> 
#include <linux/videodev2.h> // v4l2_capability struct

struct v4l2_capability;
namespace V4L2 {
	class Device;
}

/**
 * Represents the capabilities of a V4L2 device.
 */
namespace V4L2 {
class Capabilities
{
	public:
		/**
		 * CTOR Gets V4L capabilities of the device.
		 */
		Capabilities(Device* device);

		/**
		 * DTOR.
		 */
		~Capabilities();

		/**
		 * Accessors
		 */
		const char* driver();
		const char* card();
		const char* busInfo();
		const char* version();

		/**
		 * Test against bitmasks
		 */
		bool hasVideoCapture();
		bool hasReadWrite();
		bool hasStreaming();
		// TODO: More bit masks...
		
	private:
		/**
		 * V4L2 struct.
		 */
		struct v4l2_capability cap;

		Device* device;

		bool queried;

		/**
		 * Do the actual capability query.
		 */
		bool doQuery();
};
} 

#endif
