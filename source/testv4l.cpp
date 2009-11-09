#include <fcntl.h> // open
#include <unistd.h> // close
#include <linux/videodev2.h> // v4l2_capability struct
#include <stropts.h> // ioctl
#include <stdio.h>
#include <string>

const char* getPixFormat(int fourcc)
{
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
			return "Reserved";
	}

	switch(fourcc) {
		case V4L2_PIX_FMT_YUV444:
		case V4L2_PIX_FMT_YUV555:
		case V4L2_PIX_FMT_YUV565:
		case V4L2_PIX_FMT_YUV32:
			return "Packed YUV format";
	}

	switch(fourcc) {
		case V4L2_PIX_FMT_YUYV:
			return "YUYV (aka YUV 4:2:2)";
	}

	return "Unknown";
}



int main(int argc, char* argv[])
{
	int fd = 0;
	int ret = 0;
	int pixfmt = 0;
	//struct v4l2_capability* cap = 0;
	struct v4l2_capability cap;
	struct v4l2_format format;

	fd = open("/dev/video1", O_RDWR);

	printf("File descriptor: %d\n", fd);

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

	// TODO: Can be used to set/try params
	ret = ioctl(fd, VIDIOC_G_FMT, &format); 
	if(ret != 0) {
		fprintf(stderr, "There was an error in querying the camera.\n");
		return 1;
	}


	printf("Pixel Data:\n");
	printf("\twidth: %d\n", format.fmt.pix.width);
	printf("\twidth: %d\n", format.fmt.pix.height);

	pixfmt = format.fmt.pix.pixelformat;
	printf("\n\tpixel format: %s\n", getPixFormat(pixfmt));

	close(fd);
	return 0;
}

