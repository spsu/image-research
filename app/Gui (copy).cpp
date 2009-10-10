#include "Gui.hpp"
//#include <gtk/gtk.h> //(included in .hpp)
#include "../gtk/Gtk.hpp"
#include "../gtk/Window.hpp"
//#include "../cv/Image.hpp"

namespace App {

Gui::Gui(std::string title):
	window(0),
	curWidth(0),
	curHeight(0)
{
	// For drag and drop (TODO: Doesn't belong here)
	/*static const GtkTargetEntry dropTypes[] = {
		{(gchar*)"text/plain", 0, DND_TARGET_TEXT},
		{(gchar*)"image/jpg", 0, DND_TARGET_JPG},
		{(gchar*)"image/png", 0, DND_TARGET_PNG},
		{(gchar*)"x-directory/normal", 0, DND_TARGET_XDIR},
		{(gchar*)"inode/directory", 0, DND_TARGET_INODE},
		{(gchar*)"text/uri-list", 0, DND_TARGET_URILIST},
		{(gchar*)"text/x-moz-url", 0, DND_TARGET_MOZURL}
	};
	static const gint numDropTypes = sizeof(dropTypes)/sizeof(dropTypes[0]);*/

	// Init gtk
	Gtk::Gtk::init();

	// Create window
	window = new Gtk::Window(title);

	// Enable drag and drop
	/*gtk_drag_dest_set(window->getPtr(), GTK_DEST_DEFAULT_ALL, dropTypes, 
						numDropTypes, GDK_ACTION_COPY);*/

	// Destroy signals (TODO: Automate all of these.)
	g_signal_connect(window->getPtr(), "destroy", G_CALLBACK(gtk_main_quit), 0);

	// Resize signals (TODO: Automate all of these.)
	//g_signal_connect(window->getPtr(), "check-resize", 
	//					G_CALLBACK(checkResizeCb), this);

	// Drag and drop signals (TODO: Automate all of these.)
	/*g_signal_connect(window->getPtr(), "drag-motion", 
						G_CALLBACK(dragMotionCb), this);
	g_signal_connect(window->getPtr(), "drag-data-get", 
						G_CALLBACK(dragDataGetCb), this);
	g_signal_connect(window->getPtr(), "drag-data-received", 
						G_CALLBACK(dragDataReceivedCb), this);*/


	// Resizing
	// resize->addToggleCb(resizeToggleCb, this); // XXX XXX XXX WHAT TO DO?


}

Gui::~Gui()
{
	quit();
}

void Gui::start()
{
	window->showAll();
	Gtk::Gtk::start();
}

void Gui::quit()
{
	Gtk::Gtk::quit();
}

/************************* CALLBACKS ****************************/

/*void Gui::checkResizeCb(GtkContainer* container, gpointer data)
{
	Gui* self = (Gui*)data;
	std::pair<int, int> size;
	//gint width = 0;
	//gint height = 0;
	//gtk_window_get_size(GTK_WINDOW(self->window), &width, &height);

	size = self->window->getSize();

	//printf("checkResizeCb: %dx%d\n", (int)width, (int)height);

	if(size.first == self->curWidth && size.second == self->curHeight) {
		return;
	}

	// TODO TODO TODO TODO TODO
	//if(self->resize->getActive()) {
	//	self->curWidth = self.first;
	//	self->curHeight = self.second;
	//	self->image->setScale((int)self.first, (int)self.second);
	//}
}*/

/*
gboolean Gui::dragMotionCb(GtkWidget* widget, GdkDragContext* dragCtx, 
		gint x, gint y, guint time, gpointer obj)
{
	gdk_drag_status(dragCtx, GDK_ACTION_COPY, time);
	return true;
}

void Gui::dragDataGetCb(GtkWidget* widget, GdkDragContext* dragCtx, 
		GtkSelectionData* data, guint info, guint time, gpointer obj)
{
	printf("dragDataGetCb\n");
}

void Gui::dragDataReceivedCb(GtkWidget* widget, GdkDragContext* dragCtx,
		gint x, gint y, GtkSelectionData* data, guint type, 
		guint time, gpointer obj)
{
	Gui* self = (Gui*)obj;
	Cv::Image* img = 0;
	std::string filename;

	if(type == DND_TARGET_URILIST || type == DND_TARGET_MOZURL) {
		filename = (const char*)data->data;
	}
	else if(type == DND_TARGET_TEXT) {
		// TODO: My old Python code suggested I needed special handling in this
		// case. Not sure why...
		filename = (const char*)data->data;
	}

	if(filename.length() < 1) {
		fprintf(stderr, "Window drag and drop err: Filename too short.\n");
		return;
	}	

	// TODO: Do a check to ensure this is a file. Don't try to set HTTP URIs
	// unless I add libcurl or libsoup or something.
	self->image->setFile(filename);
	self->image->removeMap("original", true);

	// Backup in cache.
	img = new Cv::Image(self->image->getPixbuf()); // XXX: Potential segfault
	self->image->setMap("original", img);
}
*/

} // end namespace App
