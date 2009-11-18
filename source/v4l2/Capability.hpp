#ifndef V4L2_Capability
#define V4L2_Capability

#include <string> 
#include <linux/videodev2.h>

struct v4l2_capability;
namespace V4L2 {
	class Device;
}

/**
 * Represents the capabilities of a V4L2 device.
 * Must be associated with a particular device. 
 */
namespace V4L2 {
class Capability
{
	public:
		/**
		 * CTOR Gets V4L capabilities of the device.
		 */
		Capability(Device* device);

		/**
		 * DTOR.
		 */
		~Capability();

		/**
		 * Reset the capability structure.
		 */
		void reset();

		/**
		 * Print all information.
		 */
		void printAll();

		/**
		 * Accessors
		 */
		const char* driver();
		const char* card();
		const char* busInfo();
		int version();

		/**
		 * Test against bitmasks
		 */
		bool hasVideoCapture();
		bool hasVideoOutput();
		bool hasVideoOverlay();
		bool hasVbiCapture();
		bool hasVbiOutput();
		bool hasSlicedVbiCapture();
		bool hasSlicedVbiOutput();
		bool hasRdsCapture();
		bool hasVideoOutputOverlay();
		bool hasTuner();
		bool hasAudio();
		bool hasRadio();
		bool hasReadWrite();
		bool hasAsyncIo();
		bool hasStreaming();
		
	protected:
		/**
		 * V4L2 capability struct.
		 */
		struct v4l2_capability capability;
		
	private:

		/**
		 * Pointer to the device.
		 */
		Device* device;

		/**
		 * Whether the device has been queried.
		 */
		bool queried;

		/**
		 * Do the actual capability query.
		 */
		bool doQuery();
};
} 

#endif
