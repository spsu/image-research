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
#include <cstdlib>

/**
 * Globals.
 * TODO: Globals are bad. Make a lookup system/dictionary in App::Gui.
 */

Gtk::Button* button = 0;
App::ImagePane* imgPane = 0;

/**
 * Negative function.
 * TODO: Needs cleanup, also requires Cv::Image pixel access refactor (TODO).
 */
void negative()
{
	Gtk::Image* gtkImg = 0;
	Cv::Image* img = 0;
	int width, height;

	gtkImg = imgPane->getImage();
	img = new Cv::Image(gtkImg->getPixbuf());

	width = img->getWidth();
	height = img->getHeight();

	RgbImage pix = RgbImage(img->getPtr());

	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			pix[i][j].r = abs(pix[i][j].r - 255);
			pix[i][j].g = abs(pix[i][j].g - 255);
			pix[i][j].b = abs(pix[i][j].b - 255);
		}
	}

	gtkImg->setPixbuf(img->toPixbuf());
	delete img;
}

/**
 * Callback for conversion to negative. 
 * Changes the gui a little and dispatches to relevant function.
 */
void negativeCb(GtkButton* gtkbutton, gpointer data)
{
	// Negative
	negative();
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
	gui = new App::Gui("Negative Demo");
	imgPane = new App::ImagePane("./discovery-small.jpg");

	// Create other Gtk widgets
	vbox = new Gtk::VBox(false, 0);
	hbox = new Gtk::HBox(false, 0);
	resize = new Gtk::CheckButton("_f_it to window", true);
	button = new Gtk::Button("Negative");

	// Construct GUI
	gui->setChild(vbox);
	vbox->packStart(hbox, false, false, 0);
	hbox->packStart(resize, false, false, 0);
	vbox->packStart(imgPane->getImage(), true, true, 0);
	vbox->packStart(button, false, false, 0);

	// Install negative callback
	button->addClickedCb(negativeCb, gui);

	gui->start();

	return 0;
}
