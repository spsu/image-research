
#include <stdio.h>
#include <string>

#include "v4l2/Device.hpp"
#include "v4l2/Format.hpp"
#include "v4l2/Capabilities.hpp"

#include <stropts.h> // ioctl
#include <linux/videodev2.h>
#include <errno.h>
#include <string.h> // memset
#include <stdlib.h> // calloc
#include <sys/mman.h> // mmap
#include <vector>

#include "app/Gui.hpp"
#include "app/ImagePane.hpp"
#include "cv/Image.hpp"
#include "gtk/all.hpp"

enum queueState { ENQUEUED, DEQUEUED, NONE };

// Buffers
struct bbuffer {
	void* start;
	size_t length;
	queueState state;
};

/**
 * Globals.
 * TODO: Globals are bad. Make a lookup system/dictionary in App::Gui.
 */

Gtk::Image* gtkImg = 0;
Gtk::Button* button = 0;
App::ImagePane* imgPane = 0;
int fd = 0;
V4L2::Device* dev = 0;
V4L2::Capabilities* cap = 0;
V4L2::Format* fmt = 0;
struct v4l2_requestbuffers reqbuf;
std::vector<bbuffer> vbuffers;

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtkmain.h>

/////////////////////////////////////

static void processImage(unsigned char *p, int len)
{
	GdkPixbuf* pixbuf = 0;

	printf("Len %d\n", len);

	pixbuf = gdk_pixbuf_new_from_data(
		(const guchar*)p, // data
		GDK_COLORSPACE_RGB,
		false,
		8,
		640,
		480,
		1280,
		NULL,
		NULL
	);

	/*            (const guchar *data,
                                                         GdkColorspace colorspace,
                                                         gboolean has_alpha,
                                                         int bits_per_sample,
                                                         int width,
                                                         int height,
                                                         int rowstride,
                                                         GdkPixbufDestroyNotify destroy_fn,
                                                         gpointer destroy_fn_data);*/

	gtkImg->setPixbuf(pixbuf);
}

int prepCam()
{
	gtkImg = imgPane->getImage();

	dev = new V4L2::Device("/dev/video2");

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



	// Try to set streaming...
	printf("\nStreaming...\n");
	int ret = 0;
	reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	reqbuf.memory = V4L2_MEMORY_MMAP; // or V4L2_MEMORY_USERPTR
	// video output requires at least two buffers, one displayed and one filled 
	// by the application.
	reqbuf.count = 2; // ONLY FOR V4L2_MEMORY_MMAP

	ret = ioctl(fd, VIDIOC_REQBUFS, &reqbuf);
	if(ret == -1) {
		switch(errno) {
			case EBUSY:
				fprintf(stderr, 
					"Buffer busy err: IO in progress, or mem still mapped.\n");
				break;
			case EINVAL:
				fprintf(stderr, 
					"Buffer type or IO method unsupported.\n");
				break;
			default:
				fprintf(stderr, 
					"An unknown request buffer error occurred.\n");
		}
	}

	printf("\tNumber of buffers allocated: %d\n", reqbuf.count);


	// ================================ C-TYPE BUFFERS =========================

	// Buffers to copy to later
	/*struct {
		void *start;
		size_t length;
	} *buffersx;


	struct buff {
		void *start;
		size_t length;
	};

	struct buff *buffers;

	buffers = (buff*) calloc(reqbuf.count, sizeof(*buffers));

	for(unsigned int i = 0; i < reqbuf.count; i++) {
		struct v4l2_buffer buffer;
		memset(&buffer, 0, sizeof(buffer)); // clear buffer
		buffer.type = reqbuf.type;
		buffer.memory = reqbuf.memory;
		buffer.index = i;

		ret = ioctl(fd, VIDIOC_QUERYBUF, &buffer);
		if(ret == -1) {
			fprintf(stderr, "Querying the buffer failed\n");
			return 1;
		}


        buffers[i].length = buffer.length; // remember for munmap()

        buffers[i].start = mmap (NULL, buffer.length,
                                 PROT_READ | PROT_WRITE, // recommended
                                 MAP_SHARED,             // recommended
                                 fd, buffer.m.offset);

        if (MAP_FAILED == buffers[i].start) {
                // If you do not exit here you should unmap() and free()
                //   the buffers mapped so far. 
                perror ("mmap");
                return 1;
        }
	}*/

	// ============================ END C-TYPE BUFFERS =========================




	for(unsigned int i = 0; i < reqbuf.count; i++) {
		struct v4l2_buffer buffer;
		memset(&buffer, 0, sizeof(buffer)); // clear buffer
		buffer.type = reqbuf.type;
		buffer.memory = reqbuf.memory;
		buffer.index = i;

		ret = ioctl(fd, VIDIOC_QUERYBUF, &buffer);
		if(ret == -1) {
			fprintf(stderr, "Querying the buffer failed\n");
			return 1;
		}

		

		// TODO: TEMP!
		ret = ioctl(fd, VIDIOC_QBUF, &buffer);
		if(ret == -1) {
			switch(errno) {
				case EAGAIN:
					fprintf(stderr, "EAGAIN\n");
					break;
				case EINVAL:
					fprintf(stderr, "EINVAL\n");
					break;
				case ENOMEM:
					fprintf(stderr, "ENOMEM\n");
					break;
				case EIO:
					fprintf(stderr, "EIO\n");
					break;
				default:
					fprintf(stderr, "There was an error queueing\n");
					return 1;
			}
		}




		// make each vbuffer
		vbuffers.push_back(bbuffer());
		//memset(&vbuffers[i], 0, sizeof(vbuffers[i])); // clear buffer

		vbuffers[i].state = NONE;
        vbuffers[i].length = buffer.length; // remember for munmap()
		vbuffers[i].start = mmap(NULL, buffer.length,
                                 PROT_READ | PROT_WRITE, // recommended
                                 MAP_SHARED,             // recommended
                                 fd, buffer.m.offset);

        if (MAP_FAILED == vbuffers[i].start) {
                // If you do not exit here you should unmap() and free()
                //   the buffers mapped so far. 
                fprintf(stderr, "Memory map failed!\n");
                return 1;
        }
	}
		
	for(unsigned int i = 0; i < vbuffers.size(); i++) {
		printf("Buffer %d: Size %d\n", i, vbuffers[i].length);
	}

	dev->streamOn();
}

int doCamera()
{


	//unsigned int i = 0;
	int ret = 0;
	//while(1) {

		struct v4l2_buffer buffer;
		memset(&buffer, 0, sizeof(buffer)); // clear buffer
		buffer.type = reqbuf.type;
		buffer.memory = reqbuf.memory;
		//buffer.index = i;

		// Dequeue to process
		ret = ioctl(fd, VIDIOC_DQBUF, &buffer);
		if(ret == -1) {
			switch(errno) {
				case EAGAIN:
					fprintf(stderr, "EAGAIN\n");
					break;
				case EINVAL:
					fprintf(stderr, "EINVAL\n");
					break;
				case ENOMEM:
					fprintf(stderr, "ENOMEM\n");
					break;
				case EIO:
					fprintf(stderr, "EIO\n");
					break;
				default:
					fprintf(stderr, "There was an error dequeueing\n");
			}
		}

		processImage((unsigned char*)vbuffers[buffer.index].start, buffer.bytesused);

		// Queue it back at the camera 
		ret = ioctl(fd, VIDIOC_QBUF, &buffer);
		if(ret == -1) {
			switch(errno) {
				case EAGAIN:
					fprintf(stderr, "EAGAIN\n");
					break;
				case EINVAL:
					fprintf(stderr, "EINVAL\n");
					break;
				case ENOMEM:
					fprintf(stderr, "ENOMEM\n");
					break;
				case EIO:
					fprintf(stderr, "EIO\n");
					break;
				default:
					fprintf(stderr, "There was an error queueing\n");
					return 1;
			}
		}



		printf("Iter\n");
	//	sleep(1);

	//}

	//i = read(fd, buffers[0].start, buffers[0].length);

	// Cleanup
	//for (i = 0; i < reqbuf.count; i++)
    //	munmap (buffers[i].start, buffers[i].length);
}

void doCamera2(GtkButton* gtkbutton, gpointer data)
{
	doCamera();
}
gboolean doCamera3(gpointer data)
{
	doCamera();
	return true;
}


int main(int argc, char* argv[])
{

	App::Gui* gui = 0;
	Gtk::VBox* vbox = 0;
	Gtk::HBox* hbox = 0;
	Gtk::CheckButton* resize = 0;

	// Create main application elements
	gui = new App::Gui("Grayscale Demo");
	imgPane = new App::ImagePane("./media/example.jpg");

	// Create other Gtk widgets
	vbox = new Gtk::VBox(false, 0);
	hbox = new Gtk::HBox(false, 0);
	resize = new Gtk::CheckButton("_f_it to window", true);
	button = new Gtk::Button("Grayscale");

	// Construct GUI
	gui->setChild(vbox);
	vbox->packStart(hbox, false, false, 0);
	hbox->packStart(resize, false, false, 0);
	vbox->packStart(imgPane->getImage(), true, true, 0);
	vbox->packStart(button, false, false, 0);

	// Install grayscale callback
	button->addClickedCb(doCamera2, gui);

	prepCam();

	gtk_idle_add(doCamera3, NULL);

	gui->start();
	//doCamera();
	printf("\nDone\n");
	return 0;
}


