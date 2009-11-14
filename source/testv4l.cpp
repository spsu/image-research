
#include <stdio.h>
#include <string>

#include "v4l2/Device.hpp"
#include "v4l2/Format.hpp"
#include "v4l2/Capability.hpp"
#include "v4l2/RequestBuffers.hpp"

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
V4L2::Capability* cap = 0;
V4L2::Format* fmt = 0;
V4L2::RequestBuffers* reqbuf = 0;
std::vector<bbuffer> vbuffers;

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtkmain.h>

/////////////////////////////////////

static void processImage(unsigned char *p, int len)
{
	GdkPixbuf* pixbuf = 0;
	unsigned char* rgb;

	// Allocate RGB buffer
	rgb = (unsigned char*)malloc(len);

	printf("Len %d\n", len);
	// For YUYV information:
	// http://v4l2spec.bytesex.org/spec/r4339.htm
	// http://www.fourcc.org/fccyvrgb.php
	int j = 0;
	for(int i = 0; i < len; i+=4) {
		int y1 = p[i];
		int cb = p[i+1];
		int y2 = p[i+2];
		int cr = p[i+3];

		//R = 1.164(Y - 16) + 1.596(V - 128)
		//G = 1.164(Y - 16) - 0.813(V - 128) - 0.391(U - 128)
		//B = 1.164(Y - 16)                   + 2.018(U - 128)
		
		int r1 = (int) 1.164*(y1 - 16) + 1.596*(cr - 128);
		int g1 = (int) 1.164*(y1 - 16) - 0.813*(cr - 128) - 0.391*(cb - 128);
		int b1 = (int) 1.164*(y1 - 16) + 2.018*(cb - 128);
		
		int r2 = (int) 1.164*(y1 - 16) + 1.596*(cr - 128);
		int g2 = (int) 1.164*(y1 - 16) - 0.813*(cr - 128) - 0.391*(cb - 128);
		int b2 = (int) 1.164*(y1 - 16) + 2.018*(cb - 128);

		/*r1 &= 0xff;
		g1 &= 0xff;
		b1 &= 0xff;
		r2 &= 0xff;
		g2 &= 0xff;
		b2 &= 0xff;*/
		if(r1 > 255) {
			r1 = 255;
		}
		if(g1 > 255) {
			g1 = 255;
		}
		if(b1 > 255) {
			b1 = 255;
		}
		if(r2 > 255) {
			r2 = 255;
		}
		if(g2 > 255) {
			g2 = 255;
		}
		if(b2 > 255) {
			b2 = 255;
		}

		if(r1 < 0) {
			r1 = 0;
		}	
		if(g1 < 0) {
			g1 = 0;
		}	
		if(b1 < 0) {
			b1 = 0;
		}	
		if(r2 < 0) {
			r2 = 0;
		}	
		if(g2 < 0) {
			g2 = 0;
		}	 
		if(b2 < 0) {
			b2 = 0;
		}	

		if(r1 > 255 || g1 > 255 || b1 > 255) {
			printf("[%d] R: %d, G: %d, B: %d\n", i, r1, g1, b1);
		}
		if(r1 < 0 || g1 < 0 || b1 < 0) {
			printf("[%d] R: %d, G: %d, B: %d\n", i, r1, g1, b1);
		}
		
		/*r1 &= 255;
		g1 &= 255;
		b1 &= 255;

		if(r1 < 0) {
			r1 = 0;
		}
		if(g1 < 0) {
			g1 = 0;
		}
		if(b1 < 0) {
			b1 = 0;
		}*/

		/*r1 &= 200;
		g1 &= 200;
		b1 &= 200;
		r2 &= 200;
		g2 &= 200;
		b2 &= 200;*/

		/*int r1 = y1;
		int g1 = y1;
		int b1 = y1;
		int r2 = y2;
		int g2 = y2;
		int b2 = y2;*/

		rgb[j] = (int)r1;
		rgb[j+1] = (int)g1;
		rgb[j+2] = (int)b1;

		rgb[j+3] = (int)r2;
		rgb[j+4] = (int)g2;
		rgb[j+5] = (int)b2;

		j += 6;
	}

	pixbuf = gdk_pixbuf_new_from_data(
		//(const guchar*)p, // data
		(const guchar*)rgb,
		GDK_COLORSPACE_RGB,
		false,
		8,
		640,
		480,
		640*3, // rowstride
		NULL,
		NULL
	);

	gtkImg->setPixbuf(pixbuf);

	//free(rgb);
	rgb = NULL;
}

int prepCam()
{
	gtkImg = imgPane->getImage();

	dev = new V4L2::Device("/dev/video0");

	fd = dev->getFd();

	printf("File descriptor (main): %d\n", fd);

	cap = dev->getCapability();

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

	reqbuf = new V4L2::RequestBuffers(dev);
	reqbuf->makeRequest();

	printf("\tNumber of buffers allocated: %d\n", reqbuf->getCount());

	for(unsigned int i = 0; i < reqbuf->getCount(); i++) {
		struct v4l2_buffer buffer;
		memset(&buffer, 0, sizeof(buffer)); // clear buffer
		buffer.type = reqbuf->getType();
		buffer.memory = reqbuf->getMemory();
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
	return 0;
}

int doCamera()
{


	//unsigned int i = 0;
	int ret = 0;
	//while(1) {

		struct v4l2_buffer buffer;
		memset(&buffer, 0, sizeof(buffer)); // clear buffer
		buffer.type = reqbuf->getType();
		buffer.memory = reqbuf->getMemory();
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



	//	printf("Iter\n");
	//	sleep(1);

	//}

	//i = read(fd, buffers[0].start, buffers[0].length);

	// Cleanup
	//for (i = 0; i < reqbuf.count; i++)
    //	munmap (buffers[i].start, buffers[i].length);
	return 0;
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


