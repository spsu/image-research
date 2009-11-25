#include "DriverFrame.hpp"
#include "../Buffers.hpp"
#include "../Camera.hpp"
#include "../../wrap/Buffer.hpp"

namespace V4L2 {

DriverFrame::DriverFrame()
{
	// TODO
}

DriverFrame::~DriverFrame()
{
	// TODO
}

unsigned char* DriverFrame::getData()
{
	return (unsigned char*)buffers->getBuffer(curBuf->getIndex())->getStart();
}

int Frame::getBytesUsed()
{
	return curBuffer->getBytesUsed();
}

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
