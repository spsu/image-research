#include "DriverFrame.hpp"

namespace V4L2 {


int DriverFrame::getWidth()
{
	Camera* cam = 0;
	cam = (Camera*)buffers->getDevice();
	return cam->getFormat()->getWidth();
}

int DriverFrame::getHeight()
{
	Camera* cam = 0;
	cam = (Camera*)buffers->getDevice();
	return cam->getFormat()->getHeight();
}

} // end namespace V4L2
