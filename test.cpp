#include "app/Gui.hpp"
#include "app/ImagePane.hpp"

#include "gtk/Widget.hpp"
#include "gtk/Window.hpp"
#include "gtk/Image.hpp"
#include "gtk/box/VBox.hpp"

int main(int argc, char *argv[])
{
	App::Gui* gui = 0;
	App::ImagePane* img1 = 0;
	App::ImagePane* img2 = 0;
	Gtk::Window* win = 0;
	Gtk::Image* gimg = 0;
	Gtk::VBox* vbox = 0;

	gui = new App::Gui("Test");
	img1 = new App::ImagePane("./discovery-small.jpg");
	img2 = new App::ImagePane("./discovery-small.jpg");
	vbox = new Gtk::VBox();

	win = gui->getWindow();
	//gimg = img->getImage();
	
	//gui->setChild((Gtk::Widget*)gimg);
	gui->setChild(vbox);
	vbox->packStart(img1->getImage());
	vbox->packStart(img2->getImage());

	gui->start();

	return 0;
}
