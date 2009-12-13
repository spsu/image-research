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

			dp.push_back(Point());
			dp.push_back(Point());
			dp.push_back(Point(0, height-1));
			dp.push_back(Point(width-1, height-1));

			float xp0 = 0;
			float yp0 = sin_t * (height - 1); // irrelevant: - (cos_t* 0);
			float xp1 = width -1;
			float yp1 = yp0;

			dp[0].x = xp0;
			dp[0].y = yp0;
			dp[1].x = xp1;
			dp[1].y = yp1;

			for(unsigned int i = 0; i < 4; i++) {
				setSrc(i, sp[i].x, sp[i].y);
			}
			for(unsigned int i = 0; i < 4; i++) {
				setDst(i, dp[i].x, dp[i].y);
			}


		};


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

	//mat = cvCreateMat(3, 3, CV_32FC1);
	Perspective* p = new Perspective(height, width);
	mat = p->getMat();


	float xp1 = cos_t * (width - 1); // irrelevant: - (sin_t* 0);
	float yp1 = sin_t * (width - 1); // irrelevant: - (cos_t* 0);
	float xp3 = xp1;
	float yp3 = height - yp1;

	if((int)deg < 180) {
		yp1 = sin_t * (height - 1) * 0.3; // XXX: Hack: 0.3 is necessary so points don't converge at infinity
		yp3 = height - yp1;
	}

	//  x0		x1
	//	*********
	//	*		*
	//	*********
	//  x2		x3

	/*src[0].x = 0;
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
	dst[3].y = yp3 + yshift; //height - 1;*/

	p->setSrc(0, 0, 0);
	p->setSrc(1, width - 1, 0);
	p->setSrc(2, 0, height - 1);
	p->setSrc(3, width -1, height - 1);

	//p->setDst(0, 0, 0);
	//p->setDst(1, xp1, yp1);
	//p->setDst(2, 0, height - 1);
	//p->setDst(3, xp3, yp3);

	p->setTranslation(350, 75);

	p->setRotationY(deg);
	p->updateMat();

	//p->setRotation(deg);

	mat = p->getMat();
	

	//cvGetPerspectiveTransform(src, dst, mat);


	//p->printMat();
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

	img = new Cv::Image(700, 400);
	img->getPtr()->origin = 1;
	warpMat = rotateY2(theta, t->getWidth(), t->getHeight()); // XXX XXX XXX XXX XXX XXX
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
			break;
		
	}

	theta = scale->getValue();
	entries[0]->setText(boost::lexical_cast<std::string>(theta));
	rotateAxis(theta);
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
	button2->addClickedCb(entryCb, gui);
	buttonAuto->addClickedCb(autoRotateToggleCb, gui);
	xscale->addValueChangedCb(xscaleCb);
	yscale->addValueChangedCb(yscaleCb);
	zscale->addValueChangedCb(zscaleCb);
	gtk_idle_add(gtkIdleRotateFn, NULL); // TODO: Add to Gtk class

	vbox->packStart(xscale, true, true, 0);
	vbox->packStart(yscale, true, true, 0);
	vbox->packStart(zscale, true, true, 0);

	setupImages();

	gui->start();

	return 0;
}

