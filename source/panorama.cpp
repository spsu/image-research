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

std::vector<int> pos;

/**
 * Do panorama stitching.
 */
void panoramaStitch()
{
	Cv::Image* img = new Cv::Image(resizeImages[0]->getWidth()*2, 
									resizeImages[0]->getHeight()*2);
	Cv::Image* t = resizeImages[0];
	IplImage* cimg = 0;

	CvPoint2D32f src[4];
	CvPoint2D32f dst[4];
	CvMat* warpMat = cvCreateMat(3, 3, CV_32FC1);

	pos.clear();
	try {
		for(unsigned int i = 0; i < 8; i++) {
			pos.push_back(boost::lexical_cast<int>(entries[i]->getText()));
		}
	}
	catch(boost::bad_lexical_cast&) {
		fprintf(stderr, "Bad lexical cast\n");
		return;
	}

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
	
	cvGetPerspectiveTransform(src, dst, warpMat);
	cvWarpPerspective(t->getPtr(), img->getPtr(), warpMat);

	gtkImages[2]->setPixbuf(img->toPixbuf()); // TODO: MEMLEAK

	pos.clear();
}

void panoramaCb(GtkButton* gtkbutton, gpointer data)
{
	panoramaStitch();
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
	button = new Gtk::Button("update");

	// Entries.
	for(unsigned int i = 0; i < 8; i++) {
		entries.push_back(new Gtk::Entry());
		entries[i]->setText("0");
		entries[i]->setMaxLength(3);
		entries[i]->setWidthChars(3);
	}


	// Construct GUI
	gui->setChild(vbox);
	vbox->packStart(hbox1, false, false, 0);
	hbox1->packStart(imgPanes[0]->getImage(), true, true, 0);
	hbox1->packStart(imgPanes[1]->getImage(), true, true, 0);
	vbox->packStart(hbox2, false, false, 0);
	hbox2->packStart(imgPanes[2]->getImage(), true, true, 0);

	vbox->packStart(hbox3, false, false, 0);
	for(unsigned int i = 0; i < entries.size(); i++) {
		hbox3->packStart(entries[i], false, false, 0);
	}
	hbox3->packStart(button, false, false, 0);

	// Install grayscale callback
	button->addClickedCb(panoramaCb, gui);

	setupImages();
	//panoramaStitch();

	gui->start();

	return 0;
}

