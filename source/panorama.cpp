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
	
	cvGetPerspectiveTransform(src, dst, warpMat);
	cvWarpPerspective(t->getPtr(), img->getPtr(), warpMat);

	gtkImages[2]->setPixbuf(img->toPixbuf()); // TODO: MEMLEAK

	pos.clear();
}

/**
 * Do panorama stitching.
 */
void panoramaStitch2()
{
	std::vector<int> nums;
	std::vector<int> outNum;

	for(unsigned int i = 0; i < 8; i++) {
		outNum.push_back(0);
	}

	try {
		for(unsigned int i = 0; i < entries2.size(); i++) {
			nums.push_back(boost::lexical_cast<int>(entries2[i]->getText()));
		}
	}
	catch(boost::bad_lexical_cast&) {
		fprintf(stderr, "Bad lexical cast\n");
		return;
	}

	int a = nums[0];
	int b = nums[0] * -1;

	outNum[0] = a;
	outNum[1] = b;
	outNum[2] = b;
	outNum[3] = a;
	outNum[4] = b;
	outNum[5] = a;
	outNum[6] = a;
	outNum[7] = b;

	for(unsigned int i = 0; i < outNum.size(); i++) {
		entries[i]->setText(boost::lexical_cast<std::string>(outNum[i]));
	}

	panoramaStitch();
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

	setupImages();
	//panoramaStitch();

	gui->start();

	return 0;
}

