/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 */

#include "app/Gui.hpp"
#include "app/ImagePane.hpp"
#include "cv/Image.hpp"
#include "cv/PerspectiveTransform.hpp"
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

	Cv::PerspectiveTransform* p = 0;

	t = resizeImages[0];
	//img = new Cv::Image(700, 400);
	img = new Cv::Image(300, 300);
	img->getPtr()->origin = 1;

	width = t->getWidth();
	height = t->getHeight();

	p = new Cv::PerspectiveTransform(width, height);

	//p->setTranslation(350, 75);
	p->setTranslation(125, 125);
	p->setRotationX(xscale->getValue());
	p->setRotationY(yscale->getValue());
	p->setRotationZ(zscale->getValue());

	p->warpPerspective(t, img);

	gtkImages[2]->setPixbuf(img->toPixbuf()); // TODO: PIXBUF MEMLEAK (Or does the closure kill it?)
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
			break;
		
	}

	theta = scale->getValue();
	//entries[0]->setText(boost::lexical_cast<std::string>(theta));
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

	resizeImages[0]->resize(100, 100);
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
	//button2 = new Gtk::Button("update roll(z)/pitch(x)/yaw(y)");
	//buttonAuto = new Gtk::Button("auto rotate [turn on]");

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
	/*for(unsigned int i = 0; i < 1; i++) {
		entries.push_back(new Gtk::Entry());
		entries[i]->setText("0");
		entries[i]->setMaxLength(4);
		entries[i]->setWidthChars(4);
		hbox4->packStart(entries[i], false, false, 0);
	}

	hbox4->packStart(button2, false, false, 0);
	hbox4->packStart(buttonAuto, false, false, 0);*/

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

