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
Gtk::Button* buttonAuto = 0;
App::ImagePane* imgPane = 0;

std::vector<Cv::Image*> origImages;
std::vector<Cv::Image*> resizeImages;
std::vector<Gtk::Image*> gtkImages;
std::vector<App::ImagePane*> imgPanes;
Gtk::VBox* vbox = 0;
Gtk::HScale* hscale = 0;

std::vector<Gtk::Entry*> entries;
Gtk::Button* button2 = 0;

#define pi 3.14159265358979323846

// Not in math.h
double deg2rad(double deg)
{
	return deg * pi / 180;
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

	float xp1 = cos_t * (width - 1); // irrelevant: - (sin_t* 0);
	float yp1 = sin_t * (width - 1); // irrelevant: - (cos_t* 0);
	float xp3 = xp1;
	float yp3 = height + yp1; // value if NOT rhombus
	yp3 = height - yp1;

	/*double swap = 0.0f;
	if(yp1 - yp3 > 0.0f) {
		//ytest = height - 1;
		//xtest = width - xp;
		printf("******* TESTING ******* \n");
		//printf("\tsin: %f, cos: %f\n\typ1: %f, yp2: %f\n\n\txp1: %f xp2: %f",  sin_t, cos_t, yp1, yp2, xp1, xp2);
		//yp3 = abs(height - (sin_t * (width - 1)));

		swap = yp3;
		yp3 = yp1;
		yp1 = swap;
	}*/

	if((int)deg < 180) {
		yp1 = sin_t * (height - 1);
		yp3 = height + yp1;


		std::vector<int> xs;

		xs.push_back( sin_t * (height - 1) );
		xs.push_back( sin_t * (width - 1) );
		xs.push_back( cos_t * (height - 1) );
		xs.push_back( cos_t * (width - 1) );
		xs.push_back( sin_t * (height-1) + cos_t * (width-1) );
		xs.push_back( sin_t * (height-1) + cos_t * (1) );

		for(unsigned int i = 0; i < xs.size(); i++) {
			printf("Theta: %d, Y val: %d\n", (int)deg, xs[i]);
		}

	}
	else {



		printf("== THETA IS %d [   ] ==\n", (int)deg);
		printf("\tsin: %f cos: %f\n", sin_t, cos_t);
		printf("\txy0:\t\t\t\t\ty1\n");
		printf("\t%f, %f\t\t\t%f\n", 0.0f, 0.0f, yp1);
		printf("\txy2:\t\t\t\t\ty3\n");
		printf("\t%f, %f    \t\t%f\n",0.0f, (float)(height-1), yp3 );

	}

	//yp1: %f yp2: %f\n\txp1: %f xp2: %f\n",  sin_t, cos_t, yp1, yp2, xp1, xp2);
	printf("\n");


	//  x0		x1
	//	*********
	//	*		*
	//	*********
	//  x2		x3

	src[0].x = 0;
	src[0].y = 0;
	src[1].x = width - 1;
	src[1].y = 0;
	src[2].x = 0;
	src[2].y = height - 1;
	src[3].x = width - 1;
	src[3].y = height - 1;

	int xshift = 350;
	int yshift = 50;

	dst[0].x = 0 + xshift;
	dst[0].y = 0 + yshift;

	dst[1].x = xp1 + xshift;
	dst[1].y = yp1 + yshift;

	dst[2].x = 0 + xshift;
	dst[2].y = height - 1 + yshift;

	dst[3].x = xp3 + xshift;
	dst[3].y = yp3 + yshift; //height - 1;

	cvGetPerspectiveTransform(src, dst, mat);
	return mat;
}

	// See http://en.wikipedia.org/wiki/Rotation_matrix#Dimension_three
/**
 * Do panorama stitching.
 */
void rotateAxis(float theta)
{
	Cv::Image* img = 0;
	Cv::Image* t = 0;
	CvMat* warpMat = 0;

	t = resizeImages[0];

	img = new Cv::Image(750, 500);
	img->getPtr()->origin = 1;
	warpMat = rotateY2(theta, t->getWidth(), t->getHeight()); // TODO: MEMLEAK
	cvWarpPerspective(t->getPtr(), img->getPtr(), warpMat);
	gtkImages[2]->setPixbuf(img->toPixbuf()); // TODO: MEMLEAK	

	delete img;
}

/**
 * Callbacks
 */

void entryCb(GtkButton* gtkbutton, gpointer data)
{
	float theta = 0.0f;
	float val = 0.0f;
	try {
		theta = boost::lexical_cast<float>(entries[0]->getText());
	}
	catch(boost::bad_lexical_cast&) {
		fprintf(stderr, "Bad lexical cast\n");
		return;
	}

	//rotateAxis(theta); // XXX: The hscale callback does this for us. 
	val = (float)hscale->getValue();
	if(abs(val - theta) > 0.005) {
		hscale->setValue(theta);
	}
}

bool isDoGtkIdleRotate = false;

void autoRotateToggleCb(GtkButton* gtkbutton, gpointer data)
{
	isDoGtkIdleRotate = !isDoGtkIdleRotate;
	if(isDoGtkIdleRotate) {
		buttonAuto->setLabel("auto rotate [turn off]");
	}
	else {
		buttonAuto->setLabel("auto rotate [turn on]");
	}
}

gboolean gtkIdleRotateFn(gpointer data)
{
	static float theta = 0.0f;
	float val = 0.0f;

	if(isDoGtkIdleRotate) {
		theta += 1.0f;
		if(theta >= 360.0f) {
			theta = 0.0f;
		}
		entries[0]->setText(boost::lexical_cast<std::string>(theta));

		// XXX: hscale callback calls rotate!!
		val = (float)hscale->getValue();
		if(abs(val - theta) > 0.005) {
			hscale->setValue(theta);
		}
	}
	return true;
}

/**
 * HScale callback
 */
void hscaleCb(GtkRange* a, gpointer data)
{
	Gtk::HScale* hscale = (Gtk::HScale*)data;
	double theta = 0.0;

	theta = hscale->getValue();
	entries[0]->setText(boost::lexical_cast<std::string>(theta));
	rotateAxis(theta);
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
	button2 = new Gtk::Button("update roll(z)/pitch(x)/yaw(y)");
	buttonAuto = new Gtk::Button("auto rotate [turn on]");

	hscale = new Gtk::HScale(0.0, 360.0);
	hscale->addMark(0.0, Gtk::POS_BOTTOM, "0");
	hscale->addMark(90.0, Gtk::POS_BOTTOM, "90");
	hscale->addMark(180.0, Gtk::POS_BOTTOM, "180");
	hscale->addMark(270.0, Gtk::POS_BOTTOM, "270");
	hscale->addMark(360.0, Gtk::POS_BOTTOM, "360");

	// Construct GUI
	gui->setChild(vbox);
	vbox->packStart(hbox1, false, false, 0);
	hbox1->packStart(imgPanes[0]->getImage(), true, true, 0);
	hbox1->packStart(imgPanes[1]->getImage(), true, true, 0);
	vbox->packStart(hbox2, false, false, 0);
	hbox2->packStart(imgPanes[2]->getImage(), true, true, 0);
	vbox->packStart(hbox4, false, false, 0);

	// Entries.
	for(unsigned int i = 0; i < 1; i++) {
		entries.push_back(new Gtk::Entry());
		entries[i]->setText("0");
		entries[i]->setMaxLength(4);
		entries[i]->setWidthChars(4);
		hbox4->packStart(entries[i], false, false, 0);
	}

	hbox4->packStart(button2, false, false, 0);
	hbox4->packStart(buttonAuto, false, false, 0);

	// CALLBACKS & IDLE
	button2->addClickedCb(entryCb, gui);
	buttonAuto->addClickedCb(autoRotateToggleCb, gui);
	hscale->addValueChangedCb(hscaleCb);
	gtk_idle_add(gtkIdleRotateFn, NULL); // TODO: Add to Gtk class

	vbox->packStart(hscale, true, true, 7);

	setupImages();

	gui->start();

	return 0;
}

