#include <fcntl.h> // open
#include <unistd.h> // close
#include <linux/videodev2.h> // v4l2_capability struct
#include <stropts.h> // ioctl
#include <stdio.h>
#include <string>

const char* getPixFormat(int fourcc)
{
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

const char* getField(int f)
{
	switch(f) {
		case V4L2_FIELD_ANY:
			return "Any";
		case V4L2_FIELD_NONE:
			return "None / progressive or undetermined";
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
			return "Unknown code...\n";
	}
}

const char* getColorspace(int c)
{
	switch(c) {
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
			return "Unknown code...";
	}
}

#include "v4l2/Device.hpp"
#include "v4l2/Format.hpp"
#include "v4l2/Capabilities.hpp"

int main(int argc, char* argv[])
{
	int fd = 0;
	int ret = 0;
	int pixfmt = 0;
	//struct v4l2_capability* cap = 0;
	struct v4l2_capability cap;
	struct v4l2_format format;

	V4L2::Device* dev = 0;
	V4L2::Capabilities* capa = 0;
	V4L2::Format* fmt = 0;

	dev = new V4L2::Device("/dev/video1");

	//fd = open("/dev/video1", O_RDWR);
	fd = dev->getFd();

	printf("File descriptor (main): %d\n", fd);

	ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);
	if(ret != 0) {
		fprintf(stderr, "There was an error in querying the camera.\n");
		return 1;
	}
	printf("Driver: %s\n", cap.driver);
	printf("Card: %s\n", cap.card);
	printf("Bus info: %s\n", cap.bus_info);
	printf("Version: %2d\n", cap.version);
	printf("\nCapabilities:\n");
	if(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) {
		printf("\tCapture video\n");
	}
	if(cap.capabilities & V4L2_CAP_READWRITE) {
		printf("\tHas read and/or write\n");
	}
	if(cap.capabilities & V4L2_CAP_STREAMING) {
		printf("\tHas streaming\n");
	}
	printf("\n");


	fmt = new V4L2::Format(dev);
	printf("\twidth: %d\n", fmt->getWidth());

	/*printf("\tbuftype: %d\n", format.type);
	printf("\theight: %d\n", format.fmt.pix.height);
	// TODO: Can be used to set/try params
	ret = ioctl(fd, VIDIOC_G_FMT, &format); 
	/*if(ret != 0) {
		fprintf(stderr, "There was an error in querying the camera.\n");
		return 1;
	}*/


	/*printf("Pixel Format Data:\n");
	printf("\twidth: %d\n", format.fmt.pix.width);
	printf("\theight: %d\n", format.fmt.pix.height);
	printf("\tbytes/line: %d\n", format.fmt.pix.bytesperline);
	printf("\timage buffer size: %d\n", format.fmt.pix.sizeimage);

	pixfmt = format.fmt.pix.pixelformat;
	printf("\n\tpixel format: %s\n", 
		getPixFormat(format.fmt.pix.pixelformat));
	printf("\tcolor space: %s\n", 
		getColorspace(format.fmt.pix.colorspace));
	printf("\tpixel field: %s\n", 
		getField(format.fmt.pix.field));

	close(fd);*/
	return 0;
}

