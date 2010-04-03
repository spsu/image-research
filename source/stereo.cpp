#include <stdio.h>
#include <string>

#include "gtk/all.hpp"
#include "v4l2/all.hpp"
#include "app/Gui.hpp"
#include "app/ImagePane.hpp"
#include "cv/Image.hpp"
#include "cv/StereoBMState.hpp"

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtkmain.h>
#include <boost/lexical_cast.hpp>
#include <vector>
#include <cv.h>

/* =============================== *\
		  GLOBAL VAR DEFS
\* =============================== */

Gtk::VBox* vbox = 0;
std::vector<Gtk::Image*> gtkImages;
std::vector<App::ImagePane*> panes;

V4L2::Camera* cam1 = 0;
V4L2::Camera* cam2 = 0;
int camNum1 = 0;
int camNum2 = 0;

Cv::StereoBMState* bmState = 0;
CvStereoBMState* cvBmState = 0; // TODO TEMP

std::vector<Gtk::Entry*> entries;

bool isCalibrated = false;


/* =============================== *\
   		 Forward Declartions
\* =============================== */

void startStream(V4L2::Camera* cam);

float getEntry(unsigned int offset);

void prepCams();

void cameraIter();
void depthProjectionIter(Cv::Image* f1, Cv::Image* f2);
void cameraCalibrationIter(Cv::Image* f1, Cv::Image* f2);

gboolean cameraIterOnGtkIdle(gpointer data);


/* =============================== *\
		  HELPER FUNCTIONS
\* =============================== */

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

// get value of an entry
float getEntry(unsigned int offset)
{
	if(offset > entries.size() - 1) {
		return 0.0f;
	}
	if(entries[offset]->getLength() == 0) {
		return 0.0f;
	}
	return boost::lexical_cast<float>(entries[offset]->getText());
}



/* =============================== *\
		  PREP CAMERA
\* =============================== */

// Prepare the cameras
void prepCams()
{
	std::string c1 = "/dev/video0";
	std::string c2 = "/dev/video1";

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

	// Configure Stereo capture stuff
	bmState = new Cv::StereoBMState();
	cvBmState = bmState->getPtr();
}


/* =============================== *\
		  Main Camera Iter
\* =============================== */


void cameraIter()
{
	V4L2::DriverFrame* f1 = 0;
	V4L2::DriverFrame* f2 = 0;
	Cv::Image* frame1 = 0;
	Cv::Image* frame2 = 0;


	cam1->dequeue();
	cam2->dequeue(); 

	f1 = cam1->grabTemporaryFrame();
	f2 = cam2->grabTemporaryFrame();

	if(f1 == NULL || f2 == NULL) {
		fprintf(stderr, "A frame could not be grabbed. (MEMLEAK!)\n");
		return;
	}

	frame1 = new Cv::Image(f1);
	frame2 = new Cv::Image(f2);

	if(isCalibrated) {
		depthProjectionIter(frame1, frame2);
	}
	else {
		cameraCalibrationIter(frame1, frame2);
	}

	delete frame1;
	delete frame2;
}



/* =============================== *\
	  Cam Calibration Iteration
\* =============================== */

void cameraCalibrationIter(Cv::Image* f1, Cv::Image* f2)
{

	gtkImages[0]->setPixbuf(f1->toPixbuf());
	gtkImages[1]->setPixbuf(f2->toPixbuf());

	//cvNormalize(disparity->getPtr(), disparityNorm->getPtr(), 
	//			0, 255, CV_MINMAX);
	//gtkImages[2]->setPixbuf(disparityNorm->toPixbuf());
	//gtkImages[2]->setPixbuf(disparity->toPixbuf());


	/*CvMat* camMat1 = cvCreateMat(3, 3, CV_64F);
	CvMat* camMat2 = cvCreateMat(3, 3, CV_64F);
	CvMat* distCoeff1 = cvCreateMat(1, 5, CV_64F);
	CvMat* distCoeff2 = cvCreateMat(1, 5, CV_64F);
	CvMat* rotation = cvCreateMat(3, 3 CV_64F);
	CvMat* translation = cvCreateMat(3, 1, CV_64F);
	CvMat* essential = cvCreateMat(3, 3, CV_64F);
	CvMat* fundamental = cvCreateMat(3, 3, CV_64F);


	cvStereoCalibrate(
			objectPts,
			imgPts1, imgPts2,
			camMat1, distCoeff1, camMat2, distCoeff2,
			f1->getSize(),
			rotation,
			translation,
			essential,
			fundamental,
			cvTermCriteria(
				CV_TERMCRIT_ITER+CV_TERMCRIT_EPS,
				100,
				1e-5),
			CV_CALIB_FIX_ASPECT_RATIO + CV_CALIB_ZERO_TANGENT_DIST + 
				CV_CALIB_SAME_FOCAL_LENGTH
	);
			
	);



	CvMat* rotL = cvCreateMat(3, 3, CV_64F);
	CvMat* rotR = cvCreateMat(3, 3, CV_64F);
	CvMat* projL = cvCreateMat(3, 4, CV_64F);
	CvMat* projR = cvCreateMat(3, 4, CV_64F);
	CvMat* reproj = cvCreateMat(4, 4, CV_64F); // Reprojection into 3D

	cvStereoRectify(
			camMat1, camMat2, distCoeff1, distCoeff2, // m1,m2,d1,d2
			f1->getSize(), 
			rotation, //r, t
			translation,
			rotL,
			rotR,
			projL,
			projR,
			reproj
	);*/



}


/* =============================== *\
	 Depth Projection Iteration
\* =============================== */

void depthProjectionIter(Cv::Image* f1, Cv::Image* f2)
{
	static Cv::Image* gray1 = 0;
	static Cv::Image* gray2 = 0;
	static Cv::Image* disparity = 0;
	static Cv::Image* disparityNorm = 0;
	static Cv::Image* reprojected = 0;
	static Cv::Image* reprojectedNorm = 0;

	gtkImages[0]->setPixbuf(f1->toPixbuf());
	gtkImages[1]->setPixbuf(f2->toPixbuf());

	// Stereo stuff
	if(disparity == 0) {
		CvSize size = f1->getSize();

		gray1 = new Cv::Image(size, 1);
		gray2 = new Cv::Image(size, 1);

		disparity = new Cv::Image(size, 1, 16);
		disparityNorm = new Cv::Image(size, 1, 8);

		reprojected = new Cv::Image(size, 3, 16);
		reprojectedNorm = new Cv::Image(size, 3, 8);		
	}

	cvCvtColor(f1->getPtr(), gray1->getPtr(), CV_BGR2GRAY);
	cvCvtColor(f2->getPtr(), gray2->getPtr(), CV_BGR2GRAY);

	bmState->findCorrespondence(gray1, gray1, disparity);


	float cx, cy, cpx, f, t;

	cx = getEntry(0);
	cy = getEntry(1);
	cpx = getEntry(2);
	f = getEntry(3);
	t = getEntry(4);

	if(t == 0.0f) {
		t = 1.0f;
	}


	CvMat* q = cvCreateMat(4, 4, CV_64F); // Reprojection into 3D

	CV_MAT_ELEM(*q, float, 0, 0) = 1.0f;
	CV_MAT_ELEM(*q, float, 1, 1) = 1.0f;
	CV_MAT_ELEM(*q, float, 0, 3) = -1.0f * cx; // -c[x]
	CV_MAT_ELEM(*q, float, 1, 3) = -1.0f * cy; // -c[y]
	CV_MAT_ELEM(*q, float, 2, 3) = f; // f
	CV_MAT_ELEM(*q, float, 3, 2) = -1.0f / t; // -1/T[x]
	CV_MAT_ELEM(*q, float, 3, 3) = (cx - cpx) / t; // (c[x] - c'[x])/T[x])

	cvReprojectImageTo3D(disparity->getPtr(), reprojected->getPtr(), q);
	//cvNormalize(reprojected->getPtr(), reprojectedNorm->getPtr(), 
	//			0, 255, CV_MINMAX);
	gtkImages[2]->setPixbuf(reprojected->toPixbuf());



}


/* =============================== *\
	 Main, etc.
\* =============================== */

gboolean cameraIterOnGtkIdle(gpointer data)
{
	cameraIter();
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

	panes.push_back(new App::ImagePane(""));
	panes.push_back(new App::ImagePane(""));
	panes.push_back(new App::ImagePane(""));

	// Create other Gtk widgets
	vbox = new Gtk::VBox(false, 0);
	hbox = new Gtk::HBox(false, 0);

	// Construct GUI
	gui->setChild(vbox);
	vbox->packStart(hbox, false, false, 0);
	hbox->packStart(panes[0]->getImage(), true, true, 0);
	hbox->packStart(panes[1]->getImage(), true, true, 0);
	hbox->packStart(panes[2]->getImage(), true, true, 0);

	gtkImages.push_back(panes[0]->getImage());
	gtkImages.push_back(panes[1]->getImage());
	gtkImages.push_back(panes[2]->getImage());

	// TODO TEMP Add gtk entries for Q matrix testing.
	Gtk::HBox* hbox2 = new Gtk::HBox();
	vbox->packStart(hbox2, false, false, 0);

	// Entries.
	for(unsigned int i = 0; i < 5; i++) {
		entries.push_back(new Gtk::Entry());
		entries[i]->setText("0");
		//entries[i]->setMaxLength(4);
		//entries[i]->setWidthChars(4);
		hbox2->packStart(entries[i], false, false, 0);
	}

	prepCams();
	if(!cam1->isStreaming()) {
		fprintf(stderr, "Could not get camera 1 to stream.\n");
		return 0;
	}
	if(!cam2->isStreaming()) {
		fprintf(stderr, "Could not get camera 2 to stream.\n");
		return 0;
	}

	gtk_idle_add(cameraIterOnGtkIdle, NULL);

	gui->start();

	// Finish
	printf("\nClosing cameras...\n");
	cam1->close();
	cam2->close();

	printf("Done\n");
	return 0;
}


