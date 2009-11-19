
#include <stdio.h>
#include <string>

#include "v4l2/all.hpp" // RgbImage is *temporary*, and Buffers is a bad name...

#include <stdlib.h> // calloc
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
int camNum1 = 0;
int camNum2 = 1;
bool resize = true;

V4L2::Camera* cam1 = 0;
V4L2::Camera* cam2 = 0;


/////////////////////////////////////

static void processImage(unsigned char *p, int len)
{
	GdkPixbuf* pixbuf = 0;
	int width = cam1->getFormat()->getWidth();
	int height = cam1->getFormat()->getHeight();
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


/* ======================= IGNORE ABOVE CODE ================================ */


int prepCam()
{
	V4L2::Format* fmt = 0;
	std::string c1 = "/dev/video0";
	std::string c2 = "/dev/video1";

	// Choose device
	if(camNum1 != 0) {
		c1 = "/dev/video";
		c1 += (const char*)camNum1;
	}
	if(camNum2 != 0) {
		c2 = "/dev/video";
		c2 += (const char*)camNum2;
	}

	cam1 = new V4L2::Camera(c1);
	cam2 = new V4L2::Camera(c2);

	// Try the following format:
	fmt = cam1->getFormat();
	fmt->setWidth(320);
	fmt->setHeight(240);
	fmt->setFormat();

	fmt = cam2->getFormat();
	fmt->setWidth(320);
	fmt->setHeight(240);
	fmt->setFormat();

	cam1->printInfo();
	cam1->streamOn();
	cam2->streamOn();
	return 0;
}

int doCamera()
{
	static bool iter = true;
	V4L2::Frame* frame = 0;

	if(iter) {
		frame = cam1->grabFrame();
	}
	else {
		frame = cam2->grabFrame();
	}
	iter = !iter;

	processImage(frame->getData(), frame->getLength());

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

	if(argc > 1) {
		camNum1 = (int)argv[1];
	}
	if(argc > 2) {
		camNum2 = (int)argv[2];
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

	gtkImg = imgPane->getImage();
	prepCam();

	gtk_idle_add(doCamera3, NULL);

	gui->start();

	printf("\nDone\n");
	return 0;
}


