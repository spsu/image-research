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


/**
 * Represents an HScale that returns degree values.
 */
class DegScale : public Gtk::HScale
{
	public:
		DegScale(): 
			HScale(0.0, 360.0) 
		{
			addMark(0.0, Gtk::POS_BOTTOM, "");
			addMark(90.0, Gtk::POS_BOTTOM, "");
			addMark(180.0, Gtk::POS_BOTTOM, "");
			addMark(270.0, Gtk::POS_BOTTOM, "");
			addMark(360.0, Gtk::POS_BOTTOM, "");
			setValuePos(Gtk::POS_RIGHT);
		};

		float getRadians() { return 0.0f; };
};

DegScale* xscale = 0;
DegScale* yscale = 0;
DegScale* zscale = 0;


// ===========================================================================================

struct Point {
	float x;
	float y;
	Point(): x(0.0f), y(0.0f) {};
	Point(float xPos, float yPos): x(xPos), y(yPos) {};
};

class Perspective
{
	public:
		Perspective(int w, int h):
			width(w),
			height(h),
			xTranslation(0),
			yTranslation(0),
			outWidthConstr(-1),
			outHeightConstr(-1)
		{
			mat = cvCreateMat(3, 3, CV_32FC1);
			cvZero(mat);
		};


		void setSrc(int i, float x, float y) { 
			src[i].x = x;
			src[i].y = y;
		};
		void setDst(int i, float x, float y) { 
			dst[i].x = x;
			dst[i].y = y;
		};

		void setTranslation(int x, int y) {
			xTranslation = x;
			yTranslation = y;
		};

		void updateMat() {
			CvPoint2D32f out[4];

			// Work only with temp values
			for(unsigned int i = 0; i < 4; i++) {
				out[i].x = dst[i].x;
				out[i].y = dst[i].y;
			}

			// Shift negative values back into frame. // TODO: Interesting idea.
			/*float maxNegX = 0;
			float maxNegY = 0;
			for(unsigned int i = 0; i < 4; i++) {
				if(out[i].x < maxNegX) {
					maxNegX = out[i].x;
				}
				if(out[i].y < maxNegY) {
					maxNegY = out[i].y;
				} 
			}
			for(unsigned int i = 0; i < 4; i++) {	// TODO: May need to record that we did this
				out[i].x += abs(maxNegX);			// Since this shifts for each theta value, we'll
				out[i].y += abs(maxNegY);			// need these numbers to properly align images
			}*/

			// Add translation
			for(unsigned int i = 0; i < 4; i++) {
				out[i].x = out[i].x + xTranslation;
				out[i].y = out[i].y + yTranslation;
			}

			cvGetPerspectiveTransform(src, out, mat);
		};


		// TODO TESTING
		void setRotation(float yaw, float pitch = 0.0f, float roll = 0.0f) {

			float theta = 0.0f;
			float sin_t = 0.0f;
			float cos_t = 0.0f;

			theta = deg2rad(yaw);
			sin_t = (float)sin(theta);
			cos_t = (float)cos(theta);

			
			CvMat* temp = cvCreateMat(3, 3, CV_32FC1);

			//mat2 = cvCreateMat(3, 3, CV_32FC1);
			CV_MAT_ELEM(*temp, float, 0, 0) = 1;
			CV_MAT_ELEM(*temp, float, 0, 1) = 0;
			CV_MAT_ELEM(*temp, float, 0, 2) = 0;
			CV_MAT_ELEM(*temp, float, 1, 0) = 0;
			CV_MAT_ELEM(*temp, float, 1, 1) = cos_t;
			CV_MAT_ELEM(*temp, float, 1, 2) = sin_t;
			CV_MAT_ELEM(*temp, float, 2, 0) = 0;
			CV_MAT_ELEM(*temp, float, 2, 1) = sin_t * -1.0f;
			CV_MAT_ELEM(*temp, float, 2, 2) = cos_t;

			CvMat* temp2 = cvCreateMat(3, 1, CV_32FC1);
			CV_MAT_ELEM(*temp, float, 0, 0) = 1;
			CV_MAT_ELEM(*temp, float, 1, 0) = 0;
			CV_MAT_ELEM(*temp, float, 2, 0) = 0;


			//cvGEMM(a, b, NULL, dst);

		};

		void setRotationY(float deg) {
			float theta = 0.0f;
			float sin_t = 0.0f;
			float cos_t = 0.0f;

			theta = deg2rad(deg);
			sin_t = (float)sin(theta);
			cos_t = (float)cos(theta);

			// Original points
			std::vector<Point> sp;
			std::vector<Point> dp;

			sp.push_back(Point(0, 0));
			sp.push_back(Point(width-1, 0));
			sp.push_back(Point(0, height-1));
			sp.push_back(Point(width-1, height-1));

			dp.push_back(Point(0, 0));
			dp.push_back(Point());
			dp.push_back(Point(0, height-1));
			dp.push_back(Point());

			// XXX: Hack: 0.3 "pseudo-FOV" is necessary so points don't converge
			float xp1 = cos_t * (width - 1);
			float yp1 = sin_t * (height - 1) * 0.3; 
			float xp3 = xp1;
			float yp3 = height - yp1;

			dp[1].x = xp1;
			dp[1].y = yp1;
			dp[3].x = xp3;
			dp[3].y = yp3;

			for(unsigned int i = 0; i < 4; i++) {
				setSrc(i, sp[i].x, sp[i].y);
			}
			for(unsigned int i = 0; i < 4; i++) {
				setDst(i, dp[i].x, dp[i].y);
			}

			printf("==== POINTS ====\n");
			printf("(%f, %f)\t\t(%f, %f)\n\n", dp[0].x, dp[0].y, dp[1].x, dp[1].y);
			printf("(%f, %f)\t\t(%f, %f)\n\n\n\n", dp[2].x, dp[2].y, dp[3].x, dp[3].y);

		};


		void setRotationX(float deg) {
			float theta = 0.0f;
			float sin_t = 0.0f;
			float cos_t = 0.0f;

			theta = deg2rad(deg);
			sin_t = (float)sin(theta);
			cos_t = (float)cos(theta);

			// Original points
			std::vector<Point> sp;
			std::vector<Point> dp;

			sp.push_back(Point(0, 0));
			sp.push_back(Point(width-1, 0));
			sp.push_back(Point(0, height-1));
			sp.push_back(Point(width-1, height-1));

			dp.push_back(Point(0, 0));
			dp.push_back(Point(width-1, 0));
			dp.push_back(Point());
			dp.push_back(Point());

			// XXX: Hack: 0.3 "pseudo-FOV" is necessary so points don't converge
			float xp2 = sin_t * (width - 1) * 0.3;
			float yp2 = cos_t * (height - 1);
			float xp3 = width - 1 - xp2;
			float yp3 = yp2;

			dp[2].x = xp2;
			dp[2].y = yp2;
			dp[3].x = xp3;
			dp[3].y = yp3;

			for(unsigned int i = 0; i < 4; i++) {
				setSrc(i, sp[i].x, sp[i].y);
			}
			for(unsigned int i = 0; i < 4; i++) {
				setDst(i, dp[i].x, dp[i].y);
			}


			printf("==== POINTS ====\n");
			printf("(%f, %f)\t\t(%f, %f)\n\n", dp[0].x, dp[0].y, dp[1].x, dp[1].y);
			printf("(%f, %f)\t\t(%f, %f)\n\n\n\n", dp[2].x, dp[2].y, dp[3].x, dp[3].y);
		};

		void setRotationZ(float deg) {};


		// print the matrix
		void printMat() {
			printf("[\n");
			for(unsigned int i = 0; i < 3; i++) {
				printf("   [");
				for(unsigned int j = 0; j < 3; j++) {
					float val = (float)CV_MAT_ELEM(*mat, float, i, j);
					printf("\t%f", val);
				}
				printf("  ]\n");
			}
			printf("]\n");
		};

		CvMat* getMat() { return mat; };

	protected:
		int width;
		int height;
		int xTranslation;		// Translation, or corner origin. 
		int yTranslation;
		int outWidthConstr;		// Output image size constraints, negative if not used
		int outHeightConstr;
		CvMat* mat;
		CvPoint2D32f src[4];
		CvPoint2D32f dst[4];

};






// ===========================================================================================


	// See http://en.wikipedia.org/wiki/Rotation_matrix#Dimension_three
/**
 * Do panorama stitching.
 */
void rotateAxis(float deg, int axis = 1)
{
	Cv::Image* img = 0;
	Cv::Image* t = 0;
	int width, height;

	Perspective* p = 0;

	t = resizeImages[0];
	img = new Cv::Image(700, 400);
	img->getPtr()->origin = 1;

	width = t->getWidth();
	height = t->getHeight();

	p = new Perspective(width, height);

	p->setSrc(0, 0, 0);
	p->setSrc(1, width - 1, 0);
	p->setSrc(2, 0, height - 1);
	p->setSrc(3, width -1, height - 1);

	p->setTranslation(350, 75);

	switch(axis) {
		case 1:
			p->setRotationX(deg);
			break;
		case 2:
			p->setRotationY(deg);
			break;
		case 3:
			p->setRotationZ(deg);
	}

	p->updateMat();

	cvWarpPerspective(t->getPtr(), img->getPtr(), p->getMat());
	gtkImages[2]->setPixbuf(img->toPixbuf()); // TODO: MEMLEAK

	delete img;
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

void scaleCb(int s)
{
	Gtk::HScale* scale = 0;
	double theta = 0.0;

	switch(s) {
		case 1:
			scale = (Gtk::HScale*)xscale;
			break;
		case 2:
			scale = (Gtk::HScale*)yscale;
			break;
		case 3:
			scale = (Gtk::HScale*)zscale;
			return; // TODO TEMP
		
	}

	theta = scale->getValue();
	entries[0]->setText(boost::lexical_cast<std::string>(theta));
	rotateAxis(theta, s);
}

void xscaleCb(GtkRange* a, gpointer data) { scaleCb(1); }
void yscaleCb(GtkRange* a, gpointer data) { scaleCb(2); }
void zscaleCb(GtkRange* a, gpointer data) { scaleCb(3); }


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

	xscale = new DegScale();
	yscale = new DegScale();
	zscale = new DegScale();

	// Construct GUI
	gui->setChild(vbox);
	vbox->packStart(hbox1, false, false, 0);
	//hbox1->packStart(imgPanes[0]->getImage(), true, true, 0);
	//hbox1->packStart(imgPanes[1]->getImage(), true, true, 0);
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
	xscale->addValueChangedCb(xscaleCb);
	yscale->addValueChangedCb(yscaleCb);
	zscale->addValueChangedCb(zscaleCb);

	vbox->packStart(xscale, true, true, 0);
	vbox->packStart(yscale, true, true, 0);
	vbox->packStart(zscale, true, true, 0);

	setupImages();

	gui->start();

	return 0;
}

