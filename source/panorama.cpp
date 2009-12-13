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
			pts.push_back(Point());
			pts.push_back(Point());
			pts.push_back(Point());
			pts.push_back(Point());
			resetPts();
		};

		void resetPts() {
			pts[0].x = 0;
			pts[0].y = 0;
			pts[1].x = width - 1;
			pts[1].y = 0;
			pts[2].x = 0;
			pts[2].y = height - 1;
			pts[3].x = width - 1;
			pts[3].y = height - 1;

			src[0].x = 0;
			src[0].y = 0;
			src[1].x = width - 1;
			src[1].y = 0;
			src[2].x = 0;
			src[2].y = height - 1;
			src[3].x = width - 1;
			src[3].y = height - 1;

			dst[0].x = 0;
			dst[0].y = 0;
			dst[1].x = width - 1;
			dst[1].y = 0;
			dst[2].x = 0;
			dst[2].y = height - 1;
			dst[3].x = width - 1;
			dst[3].y = height - 1;
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
		/*void setRotation(float yaw, float pitch = 0.0f, float roll = 0.0f) {

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

		};*/

		void setRotationY(float deg) {
			float theta = 0.0f;
			float sin_t = 0.0f;
			float cos_t = 0.0f;

			theta = deg2rad(deg);
			sin_t = (float)sin(theta);
			cos_t = (float)cos(theta);

			// XXX: Hack: 0.3 "pseudo-FOV" is necessary so points don't converge
			dst[1].x = cos_t * dst[1].x; // replaces (width - 1)
			dst[1].y = dst[1].y - sin_t * (height - 1) * 0.1;
			dst[3].x = cos_t * dst[3].x;
			dst[3].y = dst[3].y + sin_t * (height - 1) * 0.1; // OLD: height - yp1;


			printf("==== POINTS AFTER Y ====\n");
			printf("(%f, %f)\t\t(%f, %f)\n\n", dst[0].x, dst[0].y, dst[1].x, dst[1].y);
			printf("(%f, %f)\t\t(%f, %f)\n\n\n\n", dst[2].x, dst[2].y, dst[3].x, dst[3].y);

		};


		void setRotationX(float deg) {
			float theta = 0.0f;
			float sin_t = 0.0f;
			float cos_t = 0.0f;

			theta = deg2rad(deg);
			sin_t = (float)sin(theta);
			cos_t = (float)cos(theta);


			printf("==== POINTS BEFORE X ====\n");
			printf("(%f, %f)\t\t(%f, %f)\n\n", dst[0].x, dst[0].y, dst[1].x, dst[1].y);
			printf("(%f, %f)\t\t(%f, %f)\n\n\n\n", dst[2].x, dst[2].y, dst[3].x, dst[3].y);

			// XXX: Hack: 0.3 "pseudo-FOV" is necessary so points don't converge

			dst[2].x = dst[2].x + (sin_t * (width - 1) * 0.1);
			dst[2].y = cos_t * dst[2].y;
			dst[3].x = dst[3].x - (sin_t * (width - 1) * 0.1);
			dst[3].y = cos_t * dst[3].y;

			printf("==== POINTS AFTER X ====\n");
			printf("(%f, %f)\t\t(%f, %f)\n\n", dst[0].x, dst[0].y, dst[1].x, dst[1].y);
			printf("(%f, %f)\t\t(%f, %f)\n\n\n\n", dst[2].x, dst[2].y, dst[3].x, dst[3].y);
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
		std::vector<Point> pts;	// Temporary points for applying multiple rotations

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
	//p->resetPts();

	p->setTranslation(350, 75);

	p->setRotationX(xscale->getValue());
	p->setRotationY(yscale->getValue());
	//p->setRotationZ(deg);

	p->updateMat();

	cvWarpPerspective(t->getPtr(), img->getPtr(), p->getMat());
	gtkImages[2]->setPixbuf(img->toPixbuf()); // TODO: MEMLEAK

	delete img;
}


/**
 * HScale callback
 */
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

