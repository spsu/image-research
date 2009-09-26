
#include "gui/Window.hpp"
#include "gui/Image.hpp"

/**
 * Implement the FFT with a Gtk gui.
 */
int main(int argc, char *argv[])
{
	Gui::Window* win = 0;

	win = new Gui::Window("Title here");

	win->getImage()->setFile("./discovery-small.jpg");

	win->start();

}

