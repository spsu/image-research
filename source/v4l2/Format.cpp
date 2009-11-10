#include "Format.hpp"
#include "Device.hpp"
#include <stropts.h> // ioctl
#include <linux/videodev2.h> // v4l2_capability struct
#include <stdio.h>
#include <errno.h>

namespace V4L2 {

Format::Format()
{
	resetStruct();
}

Format::Format(Device* dev)
{
	device = dev;
	resetStruct();
}

Format::~Format()
{

}

void Format::resetStruct()
{
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
}

int Format::getWidth()
{
	doQuery();
	return format.fmt.pix.width;
}

int Format::getHeight()
{
	doQuery();
	return format.fmt.pix.height;
}

bool Format::doQuery()
{
	int ret = 0;

	if(device == NULL) {
		return false;
	}
	if(queried) {
		return false;
	}
	queried = true;

	//printf("File descriptor (Format.cpp): %d\n", device->fd);

	//printf("test: %d\n", format.fmt.pix.width);
	//printf("test: %d\n", format.fmt.pix.width);


	ret = ioctl(device->fd, VIDIOC_G_FMT, &format);

	if(ret != 0) {
		switch(errno) {
			case EBUSY:
				fprintf(stderr,
					"Could not query camera because the device was busy.\n"); 
				break;
			case EINVAL:
				fprintf(stderr,
					"There was an error with the format struct. Debug code!\n");
				break;
			default:
				fprintf(stderr, 
					"An unknown error occured in querying the camera format\n");
		}
		return false;
	}
	return true;
}

} // end namespace V4L2
