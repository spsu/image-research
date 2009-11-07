/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 */

#include "app/Gui.hpp"
#include "app/ImagePane.hpp"
#include "cv/Image.hpp"
#include "cv/Histogram.hpp" // TODO: TEMP
#include "gtk/all.hpp"
#include <stdio.h>

/**
 * Globals.
 * TODO: Globals are bad. Make a lookup system/dictionary in App::Gui.
 */

Gtk::Button* button = 0;
App::ImagePane* imgPane = 0;

/**
 * Grayscale function.
 * TODO: Needs cleanup, also requires Cv::Image pixel access refactor (TODO).
 */
void grayscale()
{
	Gtk::Image* gtkImg = 0;
	Cv::Image* img = 0;
	Cv::Histogram* hist = 0; // TODO: TEMP TEST ONLY!!!
	int width, height;
	int r, g, b;
	int avg;

	gtkImg = imgPane->getImage();
	img = new Cv::Image(gtkImg->getPixbuf());
	hist = new Cv::Histogram(); // TODO TEMP ONLY

	width = img->getWidth();
	height = img->getHeight();

	RgbPix pix = img->getPix(); // TODO: Rename Cv::Pix

	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			//r = img[i][j].r;
			r = pix[i][j].r;
			//r = *img->pix(i, j, 0);
			g = pix[i][j].g;
			b = pix[i][j].b;

			avg = (r+g+b)/3;
			pix[i][j].r = avg;
			pix[i][j].g = avg;
			pix[i][j].b = avg;
		}
	}

	// vvv TODO TEMP ONLY vvv
	hist->calculate(img);
	printf("Bin 0: %f\n", hist->query(0));


	gtkImg->setPixbuf(img->toPixbuf());
	delete img;
}

/**
 * Callback for conversion to grayscale. 
 * Changes the gui a little and dispatches to relevant function.
 */
void grayscaleCb(GtkButton* gtkbutton, gpointer data)
{
	static bool next = 0;

	// Revert
	if(next == 1) {
		imgPane->restoreOriginal();
		button->setLabel("Grayscale");
		next = 0;
		return;
	}

	// Grayscale
	grayscale();
	button->setLabel("Revert Grayscale");
	next = 1;
}

/**
 * Main function
 * Sets up the Gui, attaches any callbacks, then starts Gtk.
 */
int main(int argc, char *argv[])
{
	App::Gui* gui = 0;
	Gtk::VBox* vbox = 0;
	Gtk::HBox* hbox = 0;
	Gtk::CheckButton* resize = 0;

	// Create main application elements
	gui = new App::Gui("Grayscale Demo");
	imgPane = new App::ImagePane("./discovery-small.jpg");

	// Create other Gtk widgets
	vbox = new Gtk::VBox(false, 0);
	hbox = new Gtk::HBox(false, 0);
	resize = new Gtk::CheckButton("_f_it to window", true);
	button = new Gtk::Button("Grayscale");

	// Construct GUI
	gui->setChild(vbox);
	vbox->packStart(hbox, false, false, 0);
	hbox->packStart(resize, false, false, 0);
	vbox->packStart(imgPane->getImage(), true, true, 0);
	vbox->packStart(button, false, false, 0);

	// Install grayscale callback
	button->addClickedCb(grayscaleCb, gui);

	gui->start();

	return 0;
}

