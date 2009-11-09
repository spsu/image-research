#include <fcntl.h> // open
#include <unistd.h> // close
#include <linux/videodev2.h> // v4l2_capability struct
#include <stropts.h> // ioctl
#include <stdio.h>

int main(int argc, char* argv[])
{
	int fd = 0;
	int ret = 0;
	//struct v4l2_capability* argp = 0;
	struct v4l2_capability argp;

	fd = open("/dev/video1", O_RDWR);

	printf("File descriptor: %d\n", fd);

	ret = ioctl(fd, VIDIOC_QUERYCAP, &argp);
	printf("Ret code: %d\n", ret);

	printf("Driver: %s\n", argp.driver);
	printf("Card: %s\n", argp.card);


	while(1) {}

	close(fd);

	

	return 0;
}

