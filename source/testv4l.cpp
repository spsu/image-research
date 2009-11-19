#include <stdio.h>
#include <string>

#include "gtk/all.hpp"
#include "v4l2/all.hpp"
#include "app/Gui.hpp"
#include "app/ImagePane.hpp"
#include "cv/Image.hpp"

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtkmain.h>

/**
 * Globals.
 * TODO: Globals are bad. Make a lookup system/dictionary in App::Gui.
 */

Gtk::Image* gtkImg = 0;
App::ImagePane* imgPane = 0;
V4L2::Camera* cam = 0;
int camNum = 0;


/////////////////////////////////////

int prepCam()
{
	V4L2::Format* fmt = 0;
	std::string c = "/dev/video0";

	// Choose device
	if(camNum != 0) {
		c = "/dev/video";
		c += (const char*)camNum;
	}
	cam = new V4L2::Camera(c);

	// Try the following format:
	fmt = cam->getFormat();
	fmt->setWidth(320);
	fmt->setHeight(240);
	fmt->setFormat();

	cam->printInfo();
	cam->streamOn();
	return 0;
}

void doCamera()
{
	Cv::Image* img = 0;

	img = new Cv::Image(cam->grabFrame());

	// ***** PROCESS HERE ***** // 

	gtkImg->setPixbuf(img->toPixbuf());
	delete img;
}


/* ======================= IGNORE BELOW CODE ================================ */

gboolean doCameraGtk(gpointer data)
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
		camNum = (int)argv[1];
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

	gtk_idle_add(doCameraGtk, NULL);

	gui->start();

	printf("\nDone\n");
	return 0;
}


