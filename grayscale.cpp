
#include <gtk/gtk.h>
#include <stdio.h>

#include "gui/Window.hpp"
#include "gui/Image.hpp"
#include "image/Image.hpp"

#include "gui/box/HBox.hpp"
#include "gui/box/VBox.hpp"
#include "gui/button/Button.hpp"
#include "gui/Label.hpp"

void grayscale(GtkButton* button, gpointer data)
{
	Gtk::Window* win = (Gtk::Window*)data;
	Gtk::Image* gtkImg = 0;
	Cv::Image* img = 0;
	GdkPixbuf* pixbuf = 0;
	int width, height;
	int r, g, b;
	int avg;

	gtkImg = win->getImage();
	pixbuf = gtkImg->getPixbuf();

	img = new Cv::Image(pixbuf);

	width = img->getWidth();
	height = img->getHeight();

	RgbImage pix = RgbImage(img->getPtr());

	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			r = pix[i][j].r;
			g = pix[i][j].g;
			b = pix[i][j].b;

			avg = (r+g+b)/3;
			pix[i][j].r = avg;
			pix[i][j].g = avg;
			pix[i][j].b = avg;
		}
	}

	gtkImg->setPixbuf(img->getPixbuf());
	delete img;
}

/**
 * Implement simple grayscale
 */
int main(int argc, char *argv[])
{
	Gtk::Window* win = 0;
	Gtk::Image* pix = 0;
	Cv::Image* img = 0;
	Gtk::VBox* vbox = 0;
	Gtk::HBox* hbox = 0;
	Gtk::Button* button = 0;

	win = new Gtk::Window("Title here");
	img = new Cv::Image("./discovery-small.jpg");

	// Customize gui
	hbox = new Gtk::HBox(false, 0);
	button = new Gtk::Button("Grayscale");

	vbox = win->getVbox();
	vbox->packStart(hbox);
	hbox->packStart(button);
	hbox->showAll();

	// Install main callback
	button->addClickedCb(grayscale, win);

	// Load image
	pix = win->getImage();
	pix->setPixbuf(img->getPixbuf());

	win->start();

}

