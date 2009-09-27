
#include "gui/Window.hpp"
#include "gui/Image.hpp"
#include "image/Image.hpp"

/**
 * Implement the FFT with a Gtk gui.
 */
int main(int argc, char *argv[])
{
	Gtk::Window* win = 0;
	Gtk::Image* pix = 0;
	Cv::Image* img = 0;

	win = new Gtk::Window("Title here");
	img = new Cv::Image("./discovery-small.jpg");

	pix = win->getImage();
	pix->setPixbuf(img->getPixbuf());
	//pix->setPixbuf(img->getPixbuf());

	//img->getPixbuf();

	//win->getImage()->setFile("./discovery-small.jpg");

	win->start();

}

