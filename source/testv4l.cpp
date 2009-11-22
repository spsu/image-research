#include <stdio.h>
#include <string>

#include "gtk/all.hpp"
#include "v4l2/all.hpp"
#include "app/Gui.hpp"
#include "app/ImagePane.hpp"
#include "cv/Image.hpp"
#include "cv/Calibration.hpp"

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtkmain.h>
#include <vector>

/**
 * Globals.
 * TODO: Globals are bad. Make a lookup system/dictionary in App::Gui.
 */

Gtk::Image* gtkImg1 = 0;
Gtk::Image* gtkImg2 = 0;
App::ImagePane* imgPane1 = 0;
App::ImagePane* imgPane2 = 0;
V4L2::Camera* cam1 = 0;
V4L2::Camera* cam2 = 0;
int camNum1 = 0;
int camNum2 = 0;

Cv::Calibration* calib1 = 0;
Cv::Calibration* calib2 = 0;

int frameDt = 0;

/////////////////////////////////////

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

	cam1->streamOn();
	cam2->streamOn();

	return 0;
}

void doCalibration(Cv::Image* frame, Cv::Calibration* calib, int camNum)
{


	// Calibrate camera
	if(!calib->isCalibrated()) {
		if(frameDt % 20 == 0) {
			if(calib->findAndDrawBoardIter(frame)) {
				printf("Found board on cam %d! %d\n", 
						camNum, calib->getNumFound());
			}
		}
		frameDt++;
		if(calib->isCalibrated()) {
			printf("Done calibrating cam %d!!\n", camNum);
		}
	}

	// Undistort the image.
	/*if(calib->isCalibrated()) {
		static bool turn = true;
		if(turn) {
			calib->undistort(frame);
		}
		turn = !turn;
	}*/
	if(calib->isCalibrated()) {
		calib->undistort(frame);
	}
}

void doCamera()
{
	Cv::Image* frame1 = 0;
	Cv::Image* frame2 = 0;
	//Cv::Image* img2 = 0;

	frame1 = new Cv::Image(cam1->grabFrame());
	frame2 = new Cv::Image(cam2->grabFrame());

	// ***** PROCESS HERE ***** // 

	if(calib1 == NULL) {
		calib1 = new Cv::Calibration(7, 6, 30); 
		calib2 = new Cv::Calibration(7, 6, 30); 
	}

	doCalibration(frame1, calib1, 1);
	doCalibration(frame2, calib2, 2);

	gtkImg1->setPixbuf(frame1->toPixbuf());
	gtkImg2->setPixbuf(frame2->toPixbuf());
	delete frame1;
	delete frame2;
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
		camNum1 = (int)argv[1];
	}
	if(argc > 2) {
		camNum2 = (int)argv[2];
	}

	// Create main application elements
	gui = new App::Gui("Stereoscopic Demo");
	imgPane1 = new App::ImagePane("");
	imgPane2 = new App::ImagePane("");

	// Create other Gtk widgets
	vbox = new Gtk::VBox(false, 0);
	hbox = new Gtk::HBox(false, 0);

	// Construct GUI
	gui->setChild(vbox);
	vbox->packStart(hbox, false, false, 0);
	hbox->packStart(imgPane1->getImage(), true, true, 0);
	hbox->packStart(imgPane2->getImage(), true, true, 0);

	gtkImg1 = imgPane1->getImage();
	gtkImg2 = imgPane2->getImage();

	prepCam();
	if(!cam1->isStreaming()) {
		fprintf(stderr, "Could not get camera 1 to stream.\n");
		return 0;
	}
	if(!cam2->isStreaming()) {
		fprintf(stderr, "Could not get camera 2 to stream.\n");
		return 0;
	}

	gtk_idle_add(doCameraGtk, NULL);

	gui->start();

	printf("\nDone\n");
	return 0;
}


