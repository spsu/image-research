#include "app/Gui.hpp"
#include "app/ImagePane.hpp"

#include "gtk/all.hpp"

int main(int argc, char *argv[])
{
	App::Gui* gui = 0;
	App::ImagePane* img = 0;
	Gtk::VBox* vbox = 0;
	Gtk::HBox* hbox = 0;
	Gtk::CheckButton* resize = 0;

	// Create main application elements
	gui = new App::Gui("Test");
	img = new App::ImagePane("./discovery-small.jpg");

	// Create other Gtk widgets
	vbox = new Gtk::VBox(false, 0);
	hbox = new Gtk::HBox(false, 0);
	resize = new Gtk::CheckButton("_f_it to window", true);

	// Construct GUI
	gui->setChild(vbox);
	vbox->packStart(hbox, false, false, 0);
	hbox->packStart(resize, false, false, 0);
	vbox->packStart(img->getImage(), true, true, 0);

	gui->start();

	return 0;
}
