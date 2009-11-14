#include "Format.hpp"
#include "Device.hpp"
#include <stropts.h> // ioctl
#include <linux/videodev2.h> // v4l2_capability struct
#include <stdio.h>
#include <errno.h>
#include <string.h> // memset

namespace V4L2 {

Format::Format()
{
	reset();
}

Format::Format(Device* dev)
{
	device = dev;
	reset();
}

Format::~Format()
{
	// Nothing
}

void Format::reset()
{
	memset(&format, 0, sizeof(format));
	queried = false;
}

bool Format::getFormat(Device* dev, bool doReset)
{
	if(device == NULL && dev == NULL) {
		fprintf(stderr, "No device to query the format.\n");
		return false;
	}
	if(dev == NULL) {
		dev = device;
	}

	if(doReset) {
		reset();
	}

	// XXX: Required to query video cams, but there are other kinds of devices.
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
	return query(dev->fd, VIDIOC_G_FMT);
}

bool Format::setFormat(Device* dev)
{
	if(device == NULL && dev == NULL) {
		fprintf(stderr, "No device to query the format.\n");
		return false;
	}
	if(dev == NULL) {
		dev = device;
	}

	// XXX: Required to query video cams, but there are other kinds of devices.
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
	return query(dev->fd, VIDIOC_S_FMT);
}

bool Format::tryFormat(Device* dev)
{
	if(device == NULL && dev == NULL) {
		fprintf(stderr, "No device to query the format.\n");
		return false;
	}
	if(dev == NULL) {
		dev = device;
	}

	// XXX: Required to query video cams, but there are other kinds of devices.
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
	return query(dev->fd, VIDIOC_TRY_FMT);
}

// ============================= TRY PARAMETERS ============================= //
void Format::setWidth(int w)
{
	// XXX XXX XXX - MUST QUERY FIRST
	format.fmt.pix.width = w;
}
void Format::setHeight(int h)
{
	// XXX XXX XXX - MUST QUERY FIRST
	format.fmt.pix.height = h;
}
// ============================= GET PARAMETERS ============================= //

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

int Format::getBytesPerLine()
{
	doQuery();
	return format.fmt.pix.bytesperline;
}

int Format::getImageSize()
{
	doQuery();
	return format.fmt.pix.sizeimage;
}

const char* Format::getPixelFormat()
{
	int fourcc = 0;

	doQuery();
	fourcc = format.fmt.pix.pixelformat;

	// I know Minoru is YUYV, so this is first.
	switch(fourcc) {
		case V4L2_PIX_FMT_YUYV:
			return "YUYV (aka YUV 4:2:2)";
	}

	switch(fourcc) {
		case V4L2_PIX_FMT_YUV444:
		case V4L2_PIX_FMT_YUV555:
		case V4L2_PIX_FMT_YUV565:
		case V4L2_PIX_FMT_YUV32:
			return "Packed YUV format";
	}

	switch(fourcc) {
		case V4L2_PIX_FMT_DV:
		case V4L2_PIX_FMT_ET61X251:
		case V4L2_PIX_FMT_HI240:
		case V4L2_PIX_FMT_HM12:
		case V4L2_PIX_FMT_MJPEG:
		case V4L2_PIX_FMT_PWC1:
		case V4L2_PIX_FMT_PWC2:
		case V4L2_PIX_FMT_SN9C10X:
		case V4L2_PIX_FMT_WNVA:	
		case V4L2_PIX_FMT_YYUV:
			return "Reserved format identifier";
	}

	return "Unknown";
}

int Format::getPixelFormatCode()
{
	doQuery();
	return (int)format.fmt.pix.pixelformat;
}

const char* Format::getColorspace()
{
	doQuery();
	switch(format.fmt.pix.colorspace) {
		case V4L2_COLORSPACE_SMPTE170M:
			return "NSTC/PAL";
		case V4L2_COLORSPACE_SMPTE240M:
			return "1125-line US HDTV";
		case V4L2_COLORSPACE_REC709:
			return "HDTV and modern devices";
		case V4L2_COLORSPACE_BT878:
			return "Broken Bt878 extents";
		case V4L2_COLORSPACE_470_SYSTEM_M:
			return "M/NSTC";
		case V4L2_COLORSPACE_470_SYSTEM_BG:
			return "625-line PAL and SECAM";
		case V4L2_COLORSPACE_JPEG:
			return "JPEG Y'CbCr";
		case V4L2_COLORSPACE_SRGB:
			return "?";
		default:
			return "Unknown colorspace code...";
	}
}

int Format::getColorspaceCode()
{
	doQuery();
	return (int)format.fmt.pix.colorspace;
}

const char* Format::getField()
{
	doQuery();
	switch(format.fmt.pix.field) {
		case V4L2_FIELD_ANY:
			return "Any";
		case V4L2_FIELD_NONE:
			return "None (progressive or undetermined)";
		case V4L2_FIELD_TOP:
			return "Top";
		case V4L2_FIELD_BOTTOM:
			return "Bottom";
		case V4L2_FIELD_INTERLACED:
			return "Interlaced";
		case V4L2_FIELD_SEQ_TB:
			return "Seq TB";
		case V4L2_FIELD_SEQ_BT:
			return "Seq BT";
		case V4L2_FIELD_ALTERNATE:
			return "Alternate";
		case V4L2_FIELD_INTERLACED_TB:
			return "Interlaced TB";
		case V4L2_FIELD_INTERLACED_BT:
			return "Interlaced BT";
		default:
			return "Unknown field code...\n";
	}
}

int Format::getFieldCode()
{
	doQuery();
	return (int)format.fmt.pix.field;
}

// ============================ PROTECTED METHODS =========================== //

bool Format::query(int fd, int request)
{
	if(ioctl(fd, request, &format) != 0) {
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

// TODO: REMOVE XXX
bool Format::doQuery()
{
	if(device == NULL) {
		return false;
	}
	if(queried) {
		return false;
	}
	getFormat(NULL, true);
	return true;
}

} // end namespace V4L2
