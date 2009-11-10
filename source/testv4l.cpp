
#include <stdio.h>
#include <string>

#include "v4l2/Device.hpp"
#include "v4l2/Format.hpp"
#include "v4l2/Capabilities.hpp"

int main(int argc, char* argv[])
{
	int fd = 0;

	V4L2::Device* dev = 0;
	V4L2::Capabilities* cap = 0;
	V4L2::Format* fmt = 0;

	dev = new V4L2::Device("/dev/video1");

	fd = dev->getFd();

	printf("File descriptor (main): %d\n", fd);

	cap = dev->getCapabilities();

	printf("\nBasic Info:\n");
	printf("\tDriver: %s\n", cap->driver());
	printf("\tCard: %s\n", cap->card());
	printf("\tBus info: %s\n", cap->busInfo());
	printf("\tVersion: %2d\n", cap->version());


	printf("\nCapabilities:\n");

	printf("\tCapture video: %s\n", 
		cap->hasVideoCapture()? "Yes" : "No");
	printf("\tread/write: %s\n", 
		cap->hasReadWrite()? "Yes" : "No");
	printf("\tstreaming: %s\n", 
		cap->hasStreaming()? "Yes" : "No");


	fmt = new V4L2::Format(dev);

	printf("\nFormat:\n");
	printf("\twidth: %d\n", fmt->getWidth());
	printf("\theight: %d\n", fmt->getHeight());
	printf("\tbytes per line: %d\n", fmt->getBytesPerLine());
	printf("\timage buffer size: %d\n", fmt->getImageSize());
	printf("\tpix format: %s\n", fmt->getPixelFormat());
	printf("\tcolorspace: %s\n", fmt->getColorspace());
	printf("\tfield: %s\n", fmt->getField());

	return 0;
}

