#include "Gtk.hpp"
#include "Widget.hpp"

namespace Gtk {

void Gtk::init()
{
	g_thread_init(0);
	gdk_threads_init();
    gtk_init(0, 0); // Omitting &argc, &argv
}

void Gtk::start()
{
	gdk_threads_enter();
	gtk_main();
	gdk_threads_leave();
}

void Gtk::quit()
{
	gtk_main_quit(); // deprecated, but oh well.
}

/*void Gtk::connect(Widget* widget, std::string sigName, GCallback handler, 
				  gpointer data)
{
	g_signal_connect(widget->getPtr(), sigName, handler, data);
}*/

} // end namesace Gtk
