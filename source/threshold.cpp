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
#include "img/threshold.hpp"
#include <stdio.h>

/**
 * Globals.
 * TODO: Globals are bad. Make a lookup system/dictionary in App::Gui.
 */

Gtk::Button* button = 0;
App::ImagePane* imgPane = 0;

/**
 * Callback for conversion. 
 * Changes the gui a little and dispatches to relevant function.
 */
void conversionCb(GtkButton* gtkbutton, gpointer data)
{
	Gtk::Image* gtkImg = 0;
	Cv::Image* img = 0;

	// Negative
	gtkImg = imgPane->getImage();
	img = new Cv::Image(gtkImg->getPixbuf());

	Img::threshold(img);

	gtkImg->setPixbuf(img->toPixbuf());
	delete img;
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

	// Create main application elements
	gui = new App::Gui("Threshold Demo");
	imgPane = new App::ImagePane("./media/example.jpg");

	// Create other Gtk widgets
	vbox = new Gtk::VBox(false, 0);
	hbox = new Gtk::HBox(false, 0);
	button = new Gtk::Button("Threshold");

	// Construct GUI
	gui->setChild(vbox);
	vbox->packStart(hbox, false, false, 0);
	vbox->packStart(imgPane->getImage(), true, true, 0);
	vbox->packStart(button, false, false, 0);

	// Install negative callback
	button->addClickedCb(conversionCb, gui);

	gui->start();

	return 0;
}

