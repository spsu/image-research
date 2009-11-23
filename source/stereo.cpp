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

std::vector<Gtk::Image*> gtkImages;
std::vector<App::ImagePane*> imgPanes;

V4L2::Camera* cam1 = 0;
V4L2::Camera* cam2 = 0;
int camNum1 = 0;
int camNum2 = 0;

Cv::Calibration* calib1 = 0;
Cv::Calibration* calib2 = 0;

int frameDt = 0;

Gtk::VBox* vbox = 0;

/////////////////////////////////////

/**
 * Config files for each camera.
 */
struct ConfigFiles
{
	std::string intrinsics;
	std::string distortion;
	std::string intrinsicsStereo;
	std::string distortionStereo;
	ConfigFiles(): intrinsics(""), distortion(""), intrinsicsStereo(""),
		distortionStereo("") {};
};

ConfigFiles* config1;
ConfigFiles* config2;

// Set the formats and stream the camera
void startStream(V4L2::Camera* cam)
{
	V4L2::Format* fmt = 0;

	// Try the following format:
	fmt = cam->getFormat();
	fmt->setWidth(320);
	fmt->setHeight(240);
	fmt->setFormat();

	cam->streamOn();
}

// Load the calibration files
void loadCalibrationFiles(Cv::Calibration* calib, ConfigFiles* cf, int camNum)
{
	calib->setCameraFrameSize(320, 240);

	if(calib->isCalibrated()) {
		printf("Already calibrated (camera %d).\n", camNum);
		return;
	}
	if(!calib->loadIntrinsics(cf->intrinsics)) {
		printf("Could not load %s.\n", cf->intrinsics.c_str());
		return;
	}
	if(!calib->loadDistortion(cf->distortion)) {
		printf("Could not load %s.\n", cf->distortion.c_str());
		return;
	}
	
	if(!calib->doGenerateMap()) {
		printf("Calibration map couldn't be generated for cam %d...\n", camNum);
		return;
	}
	printf("Calibration successfully loaded for camera %d\n", camNum);
}

// Prepare the cameras
int prepCams()
{
	std::string c1 = "/dev/video0";
	std::string c2 = "/dev/video1";

	// Camera configuration files
	config1 = new ConfigFiles();
	config2 = new ConfigFiles();

	config1->intrinsics = "./media/intrinsics1.xml";
	config1->distortion = "./media/distortion1.xml";
	config2->intrinsics = "./media/intrinsics2.xml";
	config2->distortion = "./media/distortion2.xml";

	// Choose devices 
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

	// Start each camera
	startStream(cam1);
	startStream(cam2);

	calib1 = new Cv::Calibration(7, 6, 30); 
	calib2 = new Cv::Calibration(7, 6, 30); 

	// Try to load calibrations (from file) for both cameras.
	loadCalibrationFiles(calib1, config1, 1);
	loadCalibrationFiles(calib2, config2, 2);

	return 0;
}



void doCalibration(Cv::Image* frame, Cv::Calibration* calib, int camNum)
{
	std::string intrinsicsFile = "";
	std::string distortionFile = "";

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
			calib->saveIntrinsics(intrinsicsFile);
			calib->saveDistortion(distortionFile);
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

	//doCalibration(frame1, calib1, 1);
	//doCalibration(frame2, calib2, 2);

	//gtkImg1->setPixbuf(frame1->toPixbuf());
	//gtkImg2->setPixbuf(frame2->toPixbuf());
	gtkImages[0]->setPixbuf(frame1->toPixbuf());
	gtkImages[1]->setPixbuf(frame2->toPixbuf());

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
	Gtk::HBox* hbox = 0;

	if(argc > 1) {
		camNum1 = (int)argv[1];
	}
	if(argc > 2) {
		camNum2 = (int)argv[2];
	}

	// Create main application elements
	gui = new App::Gui("Stereoscopic Demo");
	//imgPane1 = new App::ImagePane("");
	//imgPane2 = new App::ImagePane("");

	imgPanes.push_back(new App::ImagePane(""));
	imgPanes.push_back(new App::ImagePane(""));

	// Create other Gtk widgets
	vbox = new Gtk::VBox(false, 0);
	hbox = new Gtk::HBox(false, 0);

	// Construct GUI
	gui->setChild(vbox);
	vbox->packStart(hbox, false, false, 0);
	hbox->packStart(imgPanes[0]->getImage(), true, true, 0);
	hbox->packStart(imgPanes[1]->getImage(), true, true, 0);

	//gtkImg1 = imgPane1->getImage();
	//gtkImg2 = imgPane2->getImage();

	gtkImages.push_back(imgPanes[0]->getImage());
	gtkImages.push_back(imgPanes[1]->getImage());

	prepCams();
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

	// Finish
	printf("\nClosing cameras...\n");
	cam1->close();
	cam2->close();

	printf("Done\n");
	return 0;
}


