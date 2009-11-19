
#include <stdio.h>
#include <string>

#include "v4l2/all.hpp" // RgbImage is *temporary*, and Buffers is a bad name...

#include <stropts.h> // ioctl
#include <linux/videodev2.h>
#include <errno.h>
#include <string.h> // memset
#include <stdlib.h> // calloc
#include <sys/mman.h> // mmap
#include <vector>

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtkmain.h>

#include "app/Gui.hpp"
#include "app/ImagePane.hpp"
#include "cv/Image.hpp"
#include "gtk/all.hpp"


/**
 * Globals.
 * TODO: Globals are bad. Make a lookup system/dictionary in App::Gui.
 */

Gtk::Image* gtkImg = 0;
App::ImagePane* imgPane = 0;
int fd = 0;
int camNum = 0;
bool resize = true;

V4L2::Device* dev = 0;
V4L2::Capability* cap = 0;
V4L2::Format* fmt = 0;
V4L2::RequestBuffers* reqbuf = 0;
//std::vector<V4L2::Buffer*> buffers;
V4L2::Buffers* buffers = 0;

/////////////////////////////////////

static void processImage(unsigned char *p, int len)
{
	GdkPixbuf* pixbuf = 0;
	int width = dev->getFormat()->getWidth();
	int height = dev->getFormat()->getHeight();
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

	// Try the following format:
	fmt = dev->getFormat();
	fmt->setWidth(640);
	fmt->setHeight(480);
	if(resize) {
		fmt->setWidth(320);
		fmt->setHeight(240);
	}
	fmt->setFormat();

	dev->printInfo();

	buffers = new V4L2::Buffers(dev);
	if(!buffers->initBuffers()) {
		fprintf(stderr, "Could not init buffers\n");
		return 1;
	}

	dev->streamOn();
	return 0;
}

int doCamera()
{
	V4L2::Buffer buffer(buffers->getRequest());

	// Dequeue to process
	if(!buffer.dequeue(dev)) {
		return 1;
	}

	processImage(
		(unsigned char*)buffers->getBuffer(buffer.getIndex())->getStart(),
		buffer.getBytesUsed()
	);

	// Queue it back at the camera 
	if(!buffer.queue(dev)) {
		return 1;
	}

	return 0;
}


/* ======================= IGNORE BELOW CODE ================================ */

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
	std::string resizeStr = "r";

	if(argc > 1) {
		camNum = (int)argv[1];
	}
	if(argc > 2 && resizeStr == argv[2]) {
		resize = false;
	}

	// Create main application elements
	gui = new App::Gui("Grayscale Demo");
	imgPane = new App::ImagePane("./media/example.jpg");

	// Create other Gtk widgets
	vbox = new Gtk::VBox(false, 0);
	hbox = new Gtk::HBox(false, 0);

	// Construct GUI
	gui->setChild(vbox);
	vbox->packStart(hbox, false, false, 0);
	vbox->packStart(imgPane->getImage(), true, true, 0);

	prepCam();

	gtk_idle_add(doCamera3, NULL);

	gui->start();
	//doCamera();
	printf("\nDone\n");
	return 0;
}


