/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 */

#include "app/Gui.hpp"
#include "app/ImagePane.hpp"
#include "cv/Image.hpp"
#include "gtk/all.hpp"
#include <stdio.h>
#include <cv.h>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <math.h>

/**
 * Globals.
 * TODO: Globals are bad. Make a lookup system/dictionary in App::Gui.
 */

Gtk::Button* button = 0;
App::ImagePane* imgPane = 0;

std::vector<Cv::Image*> origImages;
std::vector<Cv::Image*> resizeImages;
std::vector<Gtk::Image*> gtkImages;
std::vector<Gtk::Entry*> entries;
std::vector<App::ImagePane*> imgPanes;
Gtk::VBox* vbox = 0;

std::vector<Gtk::Entry*> entries2;
Gtk::Button* button2 = 0;

#define pi 3.14159265358979323846

// Not in math.h
double deg2rad(double deg)
{
	return deg * pi / 180;
}


/**
 * Do panorama stitching.
 */
void panoramaStitch()
{
	/*Cv::Image* img = new Cv::Image(resizeImages[0]->getWidth()*2, 
									resizeImages[0]->getHeight()*2);*/
	Cv::Image* img = 0;
	Cv::Image* t = resizeImages[0];

	CvPoint2D32f src[4];
	CvPoint2D32f dst[4];
	CvMat* warpMat = cvCreateMat(3, 3, CV_32FC1);

	std::vector<int> pos;

	// For image dimension calculation
	int xHi = 0;
	int xLow = 0;
	int yHi = 0;
	int yLow = 0;
	int newWidth = 0;
	int newHeight = 0;

	try {
		for(unsigned int i = 0; i < 8; i++) {
			bool isY = (bool)(i%2); 
			int val = boost::lexical_cast<int>(entries[i]->getText());
			pos.push_back(val);

			printf("Val: %d, isX: %s\n", val, isY? "Yes" : "No");

			// Set max and min for x/y coordinates for creating proper img size
			if(isY) {
				if(i < 2 || val > yHi) {
					yHi = val;
				}
				if(i < 2 || val < yLow) {
					yLow = val;
				}
			}
			else {
				if(i < 2 || val > xHi) {
					xHi = val;
				}
				if(i < 2 || val < xLow) {
					xLow = val;
				}
			}
		}
	}
	catch(boost::bad_lexical_cast&) {
		fprintf(stderr, "Bad lexical cast\n");
		return;
	}

	newWidth = abs(xLow - xHi) + resizeImages[0]->getWidth();
	newHeight = abs(yLow - yHi) + resizeImages[0]->getHeight();

	printf("X lo: %d hi: %d\n", xLow, xHi);
	printf("Y lo: %d hi: %d\n", yLow, yHi);
	printf("Size: %d x %d\n", newWidth, newHeight);

	img = new Cv::Image(newWidth, newHeight);

	src[0].x = 0;
	src[0].y = 0;
	src[1].x = t->getWidth() - 1;
	src[1].y = 0;
	src[2].x = 0;
	src[2].y = t->getHeight() -1;
	src[3].x = t->getWidth() - 1;
	src[3].y = t->getHeight() -1;

	dst[0].x = pos[0];
	dst[0].y = pos[1];
	dst[1].x = img->getWidth() - 1 - pos[2];
	dst[1].y = pos[3];
	dst[2].x = pos[4];
	dst[2].y = img->getHeight() - 1 - pos[5];
	dst[3].x = img->getWidth() - 1 - pos[6];
	dst[3].y = img->getHeight() - 1 - pos[7];
	
	//cvGetPerspectiveTransform(src, dst, warpMat);
	// See http://en.wikipedia.org/wiki/Rotation_matrix#Dimension_three

	//float theta = 30.0;


	/*CV_MAT_ELEM(*warpMat, float, 0, 0) = ct;
	CV_MAT_ELEM(*warpMat, float, 0, 1) = st * -1.0f;
	CV_MAT_ELEM(*warpMat, float, 0, 2) = 0.0f;
	CV_MAT_ELEM(*warpMat, float, 1, 0) = st;
	CV_MAT_ELEM(*warpMat, float, 1, 1) = ct;
	CV_MAT_ELEM(*warpMat, float, 1, 2) = 0.0f;
	CV_MAT_ELEM(*warpMat, float, 2, 0) = 0.0f;
	CV_MAT_ELEM(*warpMat, float, 2, 1) = 0.0f;
	CV_MAT_ELEM(*warpMat, float, 2, 2) = 1.0f;*/

	cvWarpPerspective(t->getPtr(), img->getPtr(), warpMat);

	gtkImages[2]->setPixbuf(img->toPixbuf()); // TODO: MEMLEAK

	pos.clear();
}

CvMat* rotateX(double deg)
{
	double theta = 0.0f;
	float sin_t = 0.0f;
	float cos_t = 0.0f;
	CvMat* mat = 0;

	theta = deg2rad(deg);
	sin_t = (float)sin(theta);
	cos_t = (float)cos(theta);

	mat = cvCreateMat(3, 3, CV_32FC1);

	CV_MAT_ELEM(*mat, float, 0, 0) = 1.0f;
	CV_MAT_ELEM(*mat, float, 0, 1) = 0.0f;
	CV_MAT_ELEM(*mat, float, 0, 2) = 0.0f;
	CV_MAT_ELEM(*mat, float, 1, 0) = 0.0f;
	//CV_MAT_ELEM(*mat, float, 1, 1) = cos_t;
	//CV_MAT_ELEM(*mat, float, 1, 2) = sin_t * -1.0f;
	//CV_MAT_ELEM(*mat, float, 2, 0) = 0.0f;
	//CV_MAT_ELEM(*mat, float, 2, 1) = sin_t;
	//CV_MAT_ELEM(*mat, float, 2, 2) = cos_t;
	CV_MAT_ELEM(*mat, float, 1, 1) = cos_t;
	CV_MAT_ELEM(*mat, float, 1, 2) = sin_t;
	CV_MAT_ELEM(*mat, float, 2, 0) = 0.0f;
	CV_MAT_ELEM(*mat, float, 2, 1) = sin_t * -1.0f;
	CV_MAT_ELEM(*mat, float, 2, 2) = cos_t;

	return mat;
}

CvMat* rotateY(double deg)
{
	double theta = 0.0f;
	float sin_t = 0.0f;
	float cos_t = 0.0f;
	CvMat* mat = 0;

	theta = deg2rad(deg);
	sin_t = (float)sin(theta);
	cos_t = (float)cos(theta);

	mat = cvCreateMat(3, 3, CV_32FC1);

	CV_MAT_ELEM(*mat, float, 0, 0) = cos_t;
	CV_MAT_ELEM(*mat, float, 0, 1) = 0.0f;
	CV_MAT_ELEM(*mat, float, 0, 2) = sin_t;
	CV_MAT_ELEM(*mat, float, 1, 0) = 0.0f;
	CV_MAT_ELEM(*mat, float, 1, 1) = 1.0f;
	CV_MAT_ELEM(*mat, float, 1, 2) = 0.0f;
	CV_MAT_ELEM(*mat, float, 2, 0) = sin_t * -1.0f;
	CV_MAT_ELEM(*mat, float, 2, 1) = 0.0f;
	CV_MAT_ELEM(*mat, float, 2, 2) = cos_t;

	return mat;
}

CvMat* rotateZ(double deg)
{
	double theta = 0.0f;
	float sin_t = 0.0f;
	float cos_t = 0.0f;
	CvMat* mat = 0;

	theta = deg2rad(deg);
	sin_t = (float)sin(theta);
	cos_t = (float)cos(theta);

	mat = cvCreateMat(3, 3, CV_32FC1);

	CV_MAT_ELEM(*mat, float, 0, 0) = cos_t;
	CV_MAT_ELEM(*mat, float, 0, 1) = sin_t * -1.0f;
	CV_MAT_ELEM(*mat, float, 0, 2) = 0.0f;
	CV_MAT_ELEM(*mat, float, 1, 0) = sin_t;
	CV_MAT_ELEM(*mat, float, 1, 1) = cos_t;
	CV_MAT_ELEM(*mat, float, 1, 2) = 0.0f;
	CV_MAT_ELEM(*mat, float, 2, 0) = 0.0f;
	CV_MAT_ELEM(*mat, float, 2, 1) = 0.0f;
	CV_MAT_ELEM(*mat, float, 2, 2) = 1.0f;

	return mat;
}

CvMat* rotateY2(float deg, int width, int height)
{
	double theta = 0.0f;
	float sin_t = 0.0f;
	float cos_t = 0.0f;
	CvMat* mat = 0;
	CvPoint2D32f src[4];
	CvPoint2D32f dst[4];

	theta = deg2rad(deg);
	sin_t = (float)sin(theta);
	cos_t = (float)cos(theta);

	mat = cvCreateMat(3, 3, CV_32FC1);

	float xp = (cos_t * width - 1); // irrelevant: - (sin_t* 0);
	float yp = (sin_t * width - 1); // irrelevant: - (cos_t* 0);
	float xtest = xp;
	float ytest = height + yp; // value if NOT rhombus
	ytest = height - yp;

	printf("Height: %d, yp: %f, ytest: %f\n", height, yp, ytest);

	//float swap = 0.0f;
	if(yp - ytest > 0.0f) {
		ytest = height - 1;
		//xtest = width - xp;
	}


	src[0].x = 0;
	src[0].y = 0;
	src[1].x = width - 1;
	src[1].y = 0;
	src[2].x = 0;
	src[2].y = height - 1;
	src[3].x = width - 1;
	src[3].y = height - 1;

	int xshift = 200;
	int yshift = 50;

	dst[0].x = 0 + xshift;
	dst[0].y = 0 + yshift;
	dst[1].x = xp + xshift;
	dst[1].y = yp + yshift;
	dst[2].x = 0 + xshift;
	dst[2].y = height - 1 + yshift;
	dst[3].x = xtest + xshift;
	dst[3].y = ytest + yshift; //height - 1;

	cvGetPerspectiveTransform(src, dst, mat);
	return mat;
}

	// See http://en.wikipedia.org/wiki/Rotation_matrix#Dimension_three
/**
 * Do panorama stitching.
 */
void panoramaStitch2()
{
	float theta = 0.0f;

	Cv::Image* img = 0;
	Cv::Image* t = 0;
	CvMat* warpMat = 0;

	try {
		theta = boost::lexical_cast<float>(entries2[0]->getText());
	}
	catch(boost::bad_lexical_cast&) {
		fprintf(stderr, "Bad lexical cast\n");
		return;
	}

	t = resizeImages[0];

	img = new Cv::Image(500, 500);
	img->getPtr()->origin = 1;
	warpMat = rotateY2(theta, t->getWidth(), t->getHeight()); // TODO: MEMLEAK
	cvWarpPerspective(t->getPtr(), img->getPtr(), warpMat);
	gtkImages[2]->setPixbuf(img->toPixbuf()); // TODO: MEMLEAK	

	delete img;

}

/**
 * Callbacks
 */
void panoramaCb(GtkButton* gtkbutton, gpointer data)
{
	panoramaStitch();
}

void panoramaCb2(GtkButton* gtkbutton, gpointer data)
{
	panoramaStitch2();
}

#include <time.h>
#include <signal.h>


bool isDoRotateX = true;
float theta = 0.0f;

void panoramaCbX(GtkButton* gtkbutton, gpointer data)
{
	isDoRotateX = !isDoRotateX;
}

gboolean doRotateX(gpointer data)
{
	Cv::Image* img = 0;
	Cv::Image* t = 0;
	CvMat* warpMat = 0;

	if(isDoRotateX) {
		t = resizeImages[0];

		img = new Cv::Image(500, 500);
		warpMat = rotateY2(theta, t->getWidth(), t->getHeight()); // TODO: MEMLEAK
		cvWarpPerspective(t->getPtr(), img->getPtr(), warpMat);
		gtkImages[2]->setPixbuf(img->toPixbuf()); // TODO: MEMLEAK	

		entries2[0]->setText(boost::lexical_cast<std::string>(theta));

		theta += 1.0f;
		if(theta >= 360.0f) {
			theta = 0.0f;
		}

		delete img;
	}

	return true;
}

/**
 * Setup images.
 */
void setupImages()
{
	resizeImages.push_back(origImages[0]->copy());
	resizeImages.push_back(origImages[1]->copy());

	resizeImages[0]->resize(0.5);
	resizeImages[1]->resize(0.5);

	gtkImages[0]->setPixbuf(resizeImages[0]->toPixbuf());
	gtkImages[1]->setPixbuf(resizeImages[1]->toPixbuf());
}




/**
 * Main function
 * Sets up the Gui, attaches any callbacks, then starts Gtk.
 */
int main(int argc, char *argv[])
{
	App::Gui* gui = 0;
	Gtk::HBox* hbox1 = 0;
	Gtk::HBox* hbox2 = 0;
	Gtk::HBox* hbox3 = 0;
	Gtk::HBox* hbox4 = 0;

	// Create main application elements
	gui = new App::Gui("Panorama");
	//imgPane = new App::ImagePane("./media/example.jpg");

	// Load original images.
	origImages.push_back(new Cv::Image("./media/t0cam3.jpg"));
	origImages.push_back(new Cv::Image("./media/t0cam4.jpg"));

	// Gtk Image panes
	imgPanes.push_back(new App::ImagePane());
	imgPanes.push_back(new App::ImagePane());
	imgPanes.push_back(new App::ImagePane());

	for(unsigned int i = 0; i < imgPanes.size(); i++) {	
		gtkImages.push_back(imgPanes[i]->getImage());
	}

	// Create other Gtk widgets
	vbox = new Gtk::VBox(false, 0);
	hbox1 = new Gtk::HBox(true, 0);
	hbox2 = new Gtk::HBox(true, 0);
	hbox3 = new Gtk::HBox(false, 0);
	hbox4 = new Gtk::HBox(false, 0);
	button = new Gtk::Button("update x/y");
	button2 = new Gtk::Button("update roll(z)/pitch(x)/yaw(y)");
	Gtk::Button* buttonX = new Gtk::Button("rotate X");

	// Entries.
	for(unsigned int i = 0; i < 8; i++) {
		entries.push_back(new Gtk::Entry());
		entries[i]->setText("0");
		entries[i]->setMaxLength(4);
		entries[i]->setWidthChars(4);
	}
	for(unsigned int i = 0; i < 2; i++) {
		entries2.push_back(new Gtk::Entry());
		entries2[i]->setText("0");
		entries2[i]->setMaxLength(4);
		entries2[i]->setWidthChars(4);
	}


	// Construct GUI
	gui->setChild(vbox);
	vbox->packStart(hbox1, false, false, 0);
	hbox1->packStart(imgPanes[0]->getImage(), true, true, 0);
	hbox1->packStart(imgPanes[1]->getImage(), true, true, 0);
	vbox->packStart(hbox2, false, false, 0);
	hbox2->packStart(imgPanes[2]->getImage(), true, true, 0);

	// Entries 1
	vbox->packStart(hbox3, false, false, 0);
	for(unsigned int i = 0; i < entries.size(); i++) {
		hbox3->packStart(entries[i], false, false, 0);
	}
	hbox3->packStart(button, false, false, 0);
	button->addClickedCb(panoramaCb, gui);

	// Entries 2
	vbox->packStart(hbox4, false, false, 0);
	for(unsigned int i = 0; i < entries2.size(); i++) {
		hbox4->packStart(entries2[i], false, false, 0);
	}
	hbox4->packStart(button2, false, false, 0);
	button2->addClickedCb(panoramaCb2, gui);


	hbox4->packStart(buttonX, false, false, 0);
	buttonX->addClickedCb(panoramaCbX, gui);

	setupImages();
	//panoramaStitch();

	gtk_idle_add(doRotateX, NULL);

	gui->start();

	return 0;
}

