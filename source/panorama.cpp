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
#include "cv/types.hpp"
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
	Cv::Size outSize;

	Cv::PerspectiveTransform* p = 0;

	t = resizeImages[0];
	//img = new Cv::Image(300, 300);

	width = t->getWidth();
	height = t->getHeight();

	p = new Cv::PerspectiveTransform(width, height);

	p->setNoClipping();

	//p->setTranslation(350, 75);
	//p->setTranslation(125, 125);
	p->setRotationX(xscale->getValue());
	p->setRotationY(yscale->getValue());
	p->setRotationZ(zscale->getValue(), 50, 50);

	outSize = p->getClippedSize();

	printf("W: %d H: %d\n", outSize.width, outSize.height);

	//img = new Cv::Image(700, 400);
	img = new Cv::Image(outSize);
	img->getPtr()->origin = 1;

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


int avg(int a, int b) 
{
	return (a + b)/2;
}

enum StitchState {
	IN_NONE,
	IN_A,
	IN_B,
	IN_BOTH
};

void stitchImages(Cv::Image* src, Cv::Image* dst, int x, int y)
{
	int srcWidth = src->getWidth();
	int srcHeight = src->getHeight();
	int dstWidth = dst->getWidth();
	int dstHeight = dst->getHeight();

	RgbImage srcPix = RgbImage(src->getPtr());
	RgbImage dstPix = RgbImage(dst->getPtr());

	int k = 0;
	int l = 0;

	/*for(int i = 0; i < dstHeight; i++) {

		if(i < y || i > y + srcHeight) {
			continue;
		}


		for(int j = 0; j < dstWidth; j++) {

			if(j < x || j > x + srcWidth) {
				continue;
			}

			int dr = dstPix[i][j].r;
			int dg = dstPix[i][j].g;
			int db = dstPix[i][j].b;


			int sr = srcPix[l][k].r;
			int sg = srcPix[l][k].g;
			int sb = srcPix[l][k].b;

			if(dr == 0 && dg == 0 && db == 0) {
				dstPix[i][j].r = sr;
				dstPix[i][j].g = sg;
				dstPix[i][j].b = sb;
			}
			else {
				dstPix[i][j].r = avg(sr, dr);
				dstPix[i][j].g = avg(sg, dg);
				dstPix[i][j].b = avg(sb, db);
			}

			l++;
			if(l >= srcWidth) {
				l = 0;
				k++;
			}

		}
	}*/

	for(int i = 0; i < srcHeight; i++) {
		for(int j = 0; j < srcWidth; j++) {

			int sr = srcPix[i][j].r;
			int sg = srcPix[i][j].g;
			int sb = srcPix[i][j].b;

			int dr = dstPix[i+y][j+x].r;
			int dg = dstPix[i+y][j+x].g;
			int db = dstPix[i+y][j+x].b;

			if(sr == 0 || sg == 0 || sb == 0) {
				continue;
			}

			if(dr != 0 || dg != 0 || db != 0) {
				sr = avg(sr, dr);
				sg = avg(sg, dg);
				sb = avg(sb, db);
			}
			
			dstPix[i+y][j+x].r = sr;
			dstPix[i+y][j+x].g = sg;
			dstPix[i+y][j+x].b = sb;

		}
	}

}


/**
 * Main alignment.
 */
void final()
{
	Cv::Image* t1 = 0;
	Cv::Image* t2 = 0;
	Cv::Image* tmp1 = 0;
	Cv::Image* tmp2 = 0;
	Cv::Image* fin = 0;

	Cv::PerspectiveTransform* p1 = 0;
	Cv::PerspectiveTransform* p2 = 0;

	double alpha = 0.0;
	double beta = 0.0;
	double gamma = 0.0;

	t1 = resizeImages[0];
	t2 = resizeImages[1];

	p1 = new Cv::PerspectiveTransform(t1->getSize());
	p2 = new Cv::PerspectiveTransform(t2->getSize());

	p1->setNoClipping();
	p2->setNoClipping();

	p1->setRotationX(-4);
	p1->setRotationY(-4);
	p1->setRotationZ(-2.7);

	p2->setRotationX(-2);
	p2->setRotationY(2.5);
	p2->setRotationZ(2.0);

	tmp1 = new Cv::Image(p1->getClippedSize());
	tmp1->getPtr()->origin = 1;

	tmp2 = new Cv::Image(p2->getClippedSize());
	tmp2->getPtr()->origin = 1;

	p1->warpPerspective(t1, tmp1);
	p2->warpPerspective(t2, tmp2);

	fin = new Cv::Image(800, 600);

	int xoff = 0;
	int yoff = 0;

	printf("Width: %d, Height: %d\n", fin->getWidth(), fin->getHeight());

	/*tmp1->setRoi(0, 0, tmp1->getWidth(), tmp1->getHeight());
	fin->setRoi(xoff + 75, yoff + 32, tmp1->getWidth(), tmp1->getHeight());

	alpha = 0.0;
	beta = 1.0;

	cvAddWeighted(fin->getPtr(), alpha, tmp1->getPtr(), beta, gamma, fin->getPtr());

	tmp2->setRoi(0, 0, tmp2->getWidth(), tmp2->getHeight());
	fin->resetRoi();
	fin->setRoi(xoff + 250, yoff + 0, tmp2->getWidth(), tmp2->getHeight());

	alpha = 0.5;
	beta = 1.0;
	gamma = 0.5;

	//cvAddWeighted(fin->getPtr(), alpha, tmp2->getPtr(), beta, gamma, fin->getPtr());
	cvOr(fin->getPtr(), tmp2->getPtr(), fin->getPtr());

	fin->resetRoi();*/

	stitchImages(tmp1, fin, 75, 42);
	stitchImages(tmp2, fin, 238, 0);


	gtkImages[2]->setPixbuf(fin->toPixbuf()); // TODO: PIXBUF MEMLEAK (Or does the closure kill it?)


	delete tmp1;
	delete tmp2;
}


/**
 * Setup images.
 */
void setupImages()
{
	resizeImages.push_back(origImages[0]->copy());
	resizeImages.push_back(origImages[1]->copy());

	//resizeImages[0]->resize(100, 100);
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
	final();

	gui->start();

	return 0;
}

