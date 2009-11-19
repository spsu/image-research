#include "Image.hpp"
#include <stdio.h>

namespace Gtk {

Image::Image(): 
	Widget()
{
	widget = gtk_image_new();
}

Image::Image(GdkPixbuf* pixbuf):
	Widget()
{
	widget = gtk_image_new_from_pixbuf(pixbuf);
}

Image::~Image()
{
	if(widget != NULL) {
		gtk_widget_destroy(widget);
	}
}

GdkPixbuf* Image::getPixbuf()
{
	if(GTK_IMAGE_PIXBUF != gtk_image_get_storage_type(GTK_IMAGE(widget))) {
		printf("Gtk::Image::getPixbuf() err: No internal pixbuf!\n");
		return NULL;
	}
	return gtk_image_get_pixbuf(GTK_IMAGE(widget));
}

void Image::setPixbuf(GdkPixbuf* pixbuf)
{
	GdkPixbuf* oldPb = 0;

	if(pixbuf == NULL) {
		fprintf(stderr, "Image::setPixbuf() err: Pixbuf is null!\n");
		return;
	}

	// DO NOT DO THIS WITHIN THE UI THREAD!! ONLY OTHER THREADS USING GDK.
	// SEE: http://blogs.operationaldynamics.com/andrew/software/gnome-desktop/
	// gtk-thread-awareness.html
	//gdk_threads_enter(); // If I remember, this lead to deadlocks...
	if(GTK_IMAGE_PIXBUF == gtk_image_get_storage_type(GTK_IMAGE(widget))) {
		oldPb = gtk_image_get_pixbuf(GTK_IMAGE(widget));
	}

	gtk_image_set_from_pixbuf(GTK_IMAGE(widget), pixbuf);

	if(oldPb != NULL) {
		g_object_unref(oldPb); // TODO: Make sure new pixbuf got set
	}
	//gdk_threads_leave(); // If I remember, this lead to deadlocks...
}

bool Image::setFile(std::string filename)
{
	size_t pos = 0;
	GdkPixbuf* pixbuf = 0;
	GError** err = 0;

	// Remove file:// from the URI.
	//printf("Old Filename: %s.\n", filename.c_str());
	if(filename.substr(0, 7) == "file://") {
		filename = filename.substr(7, filename.length()-7);
	}

	// Remove leading/trailing whitespace.
	pos = filename.find_first_not_of("\n\r\t ");
	filename.erase(0, pos);
	pos = filename.find_last_not_of("\n\r\t ");
	filename.erase(pos+1);
	//printf("New Filename: %s.\n", filename.c_str());

	// No loading from the Internet just yet.
	if(filename.substr(0, 7) == "http://" ||
	   filename.substr(0, 6) == "ftp://") {
			fprintf(stderr, "Image::setFile() err: Cannot load over the "
							"internet (yet)!\n");
		return false;
	}

	// Load via pixbuf loader to detect errors
	pixbuf = gdk_pixbuf_new_from_file(filename.c_str(), err);
	if(pixbuf == NULL || err != NULL) {
		fprintf(stderr, "Image::setFile() err: Error loading image.\n");
		return false;
	}

	gtk_image_set_from_pixbuf(GTK_IMAGE(widget), pixbuf);
	g_object_unref(pixbuf);
	return true;
}

} // end namespace Gtk
