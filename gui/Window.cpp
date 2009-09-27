/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 * 
 * The main gtk window. (Fairly rigid and unflexible class, however.)
 */

#include "Window.hpp"
#include "Image.hpp"
#include "CheckButton.hpp"
#include "ToggleButton.hpp"
#include "Widget.hpp"
#include "HBox.hpp"
#include "VBox.hpp"
#include <stdlib.h>

namespace Gtk {

Window::Window(std::string title):
	window(0),
	image(0),
	resize(0),
	curWidth(0),
	curHeight(0)
{
	HBox* hbox = 0;
	VBox* vbox = 0;

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

	vbox = new VBox(false, 0);
	hbox = new HBox(false, 0);

	vbox->packStart(hbox, false, false, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox->getPtr());

	image = new Image();
	vbox->packStart(image->getPtr(), true, true, 0);
	
	resize = new CheckButton("_f_it to window", true);
	hbox->packStart(resize, false, false, 0);

	gtk_widget_show_all(window);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);

	// Resizing
	resize->addToggleCb(resizeToggleCb, this);
	g_signal_connect(window, "check-resize", G_CALLBACK(checkResizeCb), this);

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

	// TODO: Better placement and initial values
	gtk_widget_set_size_request(window, 150, 150);
	gtk_window_resize(GTK_WINDOW(window), 150, 150);

	gtk_main();
	gdk_threads_leave();
}

void Window::setTitle(std::string title)
{
	gtk_window_set_title(GTK_WINDOW(window), title.c_str());
}

Image* Window::getImage()
{
	return image;
}

void Window::checkResizeCb(GtkContainer* container, gpointer data)
{
	Window* self = (Window*)data;
	gint width = 0;
	gint height = 0;

	gtk_window_get_size(GTK_WINDOW(self->window), &width, &height);

	printf("checkResizeCb: %dx%d\n", (int)width, (int)height);

	if(width == self->curWidth && height == self->curHeight) {
		return;
	}

	if(self->resize->getActive()) {
		self->curWidth = width;
		self->curHeight = height;
		self->image->setScale((int)width, (int)height);
	}
}

void Window::resizeToggleCb(GtkToggleButton* button, gpointer data)
{
	Window* self = (Window*)data;

	if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button))) {
		self->image->removeScaling();
		self->curWidth = 0;
		self->curHeight = 0;
	}
	else {
		gtk_container_check_resize(GTK_CONTAINER(self->window));
	}
}

} // end namespace Gtk
