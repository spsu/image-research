#include "Window.hpp"
#include "Image.hpp"
#include <stdlib.h>

namespace Gui {

Window::Window(std::string title):
	window(0),
	image(0)
{
	GtkWidget* hbox = 0;
	GtkWidget* vbox = 0;

	// For drag and drop
	static const GtkTargetEntry dropTypes[] = {
		{(gchar*)"text/plain", 0, DND_TARGET_TEXT},
		{(gchar*)"image/jpg", 0, DND_TARGET_JPG},
		{(gchar*)"image/png", 0, DND_TARGET_PNG},
		{(gchar*)"x-directory/normal", 0, DND_TARGET_XDIR},
		{(gchar*)"inode/directory", 0, DND_TARGET_INODE},
		{(gchar*)"text/uri-list", 0, DND_TARGET_URILIST},
		{(gchar*)"text/x-moz-url", 0, DND_TARGET_MOZURL}
	};
	static const gint numDropTypes = sizeof(dropTypes)/sizeof(dropTypes[0]);


	// Just in case
	g_thread_init(0);
	gdk_threads_init();

    gtk_init(0, 0); // Omitting &argc, &argv

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), title.c_str());

	vbox = gtk_vbox_new(false, 0);
	hbox = gtk_hbox_new(false, 0);

	gtk_box_pack_start(GTK_BOX(vbox), hbox, false, false, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	image = new Gui::Image();
	gtk_box_pack_start(GTK_BOX(vbox), image->getPtr(), true, true, 0);
	
	gtk_widget_show_all(window);

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);

	// Drag and Drop images
	gtk_drag_dest_set(window, GTK_DEST_DEFAULT_ALL, dropTypes, numDropTypes, 
						GDK_ACTION_COPY);

	g_signal_connect(window, "drag-motion", G_CALLBACK(dragMotionCb), this);
	g_signal_connect(window, "drag-data-get", G_CALLBACK(dragDataGetCb), this);
	g_signal_connect(window, "drag-data-received", 
					G_CALLBACK(dragDataReceivedCb), this);
}

gboolean Window::dragMotionCb(GtkWidget* widget, GdkDragContext* dragCtx, 
		gint x, gint y, guint time, gpointer obj)
{
	gdk_drag_status(dragCtx, GDK_ACTION_COPY, time);
	return true;
}

void Window::dragDataGetCb(GtkWidget* widget, GdkDragContext* dragCtx, 
		GtkSelectionData* data, guint info, guint time, gpointer obj)
{
	printf("dragDataGetCb\n");
}

void Window::dragDataReceivedCb(GtkWidget* widget, GdkDragContext* dragCtx,
		gint x, gint y, GtkSelectionData* data, guint type, 
		guint time, gpointer obj)
{
	std::string filename;
	Window* self = (Window*)obj;

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
}

Window::~Window()
{
	gtk_main_quit();
}

void Window::start()
{
	gdk_threads_enter();
	gtk_main();
	gdk_threads_leave();
}

void Window::setTitle(std::string title)
{
	gtk_window_set_title(GTK_WINDOW(window), title.c_str());
}

Gui::Image* Window::getImage()
{
	return image;
}

} // end namespace Gui
