
#include <stdio.h>
#include <string>

#include "v4l2/Device.hpp"
#include "v4l2/Format.hpp"
#include "v4l2/Capability.hpp"
#include "v4l2/RequestBuffers.hpp"
#include "v4l2/Buffer.hpp"
#include "v4l2/RgbImage.hpp" // XXX: Temp test

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
int camNum = 0;

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtkmain.h>

/////////////////////////////////////

static void processImage(unsigned char *p, int len)
{
	GdkPixbuf* pixbuf = 0;
	int width = fmt->getWidth();
	int height = fmt->getHeight();
	RgbImage2* rgb = new RgbImage2(width, height);

	rgb->setFromYuyv((const unsigned char*)p, len);

	pixbuf = gdk_pixbuf_new_from_data(
		(const guchar*)rgb->data,
		GDK_COLORSPACE_RGB,
		false,
		8,
		width,
		height,
		width*3, 				// rowstride
		RgbImage2::destroy,		// closure func
		rgb						// closure data
	);

	gtkImg->setPixbuf(pixbuf);
}

int prepCam()
{
	std::string cam = "/dev/video0";
	if(camNum != 0) {
		cam = "/dev/video";
		cam += (const char*)camNum;
	}

	gtkImg = imgPane->getImage();

	printf("Using device: %s\n", cam.c_str());
	dev = new V4L2::Device(cam);

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

	// Try format:
	fmt->setWidth(320);
	fmt->setHeight(240);
	fmt->setFormat();

	// Try to set streaming...
	printf("\nStreaming...\n");
	int ret = 0;

	reqbuf = new V4L2::RequestBuffers(dev);
	reqbuf->makeRequest();

	printf("\tNumber of buffers allocated: %d\n", reqbuf->getCount());

	for(unsigned int i = 0; i < reqbuf->getCount(); i++) {
		V4L2::Buffer buffer(reqbuf, i);

		if(!buffer.query(dev)) {
			return 1;
		}

		if(!buffer.queue(dev)) {
			return 1;
		}

		// make each vbuffer
		vbuffers.push_back(bbuffer());
		//memset(&vbuffers[i], 0, sizeof(vbuffers[i])); // clear buffer

		vbuffers[i].state = NONE;
        vbuffers[i].length = buffer.getLength(); // remember for munmap()
        vbuffers[i].start = mmap(NULL, buffer.getLength(),
                                 PROT_READ | PROT_WRITE, // recommended
                                 MAP_SHARED,             // recommended
								 fd, buffer.getOffset());

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
	//while(1) {

		V4L2::Buffer buffer(reqbuf);

		// Dequeue to process
		if(!buffer.dequeue(dev)) {
			return 1;
		}

		processImage((unsigned char*)vbuffers[buffer.getIndex()].start, 
				buffer.getBytesUsed());

		// Queue it back at the camera 
		if(!buffer.queue(dev)) {
			return 1;
		}

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

	if(argc > 1) {
		camNum = (int)argv[1];
	}

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


