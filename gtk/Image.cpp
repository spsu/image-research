#include "Image.hpp"
#include <stdio.h>

// TODO: This shouldn't be here, or the semantics of the class should be split 
// into two parts: Representing GtkWindow and representing the project. XXX
#include "../cv/Image.hpp" 

namespace Gtk {

Image::Image(): 
	Widget(),
	unscaled(0)
{
	widget = gtk_image_new();
}

Image::Image(GdkPixbuf* pixbuf):
	Widget(),
	unscaled(0)
{
	widget = gtk_image_new_from_pixbuf(pixbuf);
}

Image::~Image()
{
	if(widget != NULL) {
		gtk_widget_destroy(widget);
	}
	if(unscaled != NULL) {
		g_object_unref(unscaled);
	}
}

GdkPixbuf* Image::getPixbuf()
{
	if(GTK_IMAGE_PIXBUF != gtk_image_get_storage_type(GTK_IMAGE(widget))) {
		printf("Gtk::Image::getPixbuf() err: No internal pixbuf!\n");
		return NULL;
	}

	// TODO: Should I return the cached unscaled image if it exists?
	// probably not...
	return gtk_image_get_pixbuf(GTK_IMAGE(widget));
}

void Image::setPixbuf(GdkPixbuf* pixbuf)
{
	GdkPixbuf* oldPb = 0;

	if(pixbuf == NULL) {
		fprintf(stderr, "Image::setPixbuf() err: Pixbuf is null!\n");
		return;
	}

	gdk_threads_enter();
	if(GTK_IMAGE_PIXBUF == gtk_image_get_storage_type(GTK_IMAGE(widget))) {
		oldPb = gtk_image_get_pixbuf(GTK_IMAGE(widget));
	}

	gtk_image_set_from_pixbuf(GTK_IMAGE(widget), pixbuf);

	if(unscaled != NULL) {
		g_object_unref(unscaled);
	}

	if(oldPb != NULL) {
		g_object_unref(oldPb); // TODO: Make sure new pixbuf got set
	}
	gdk_threads_leave();
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

	if(unscaled != NULL) {
		g_object_unref(unscaled);
	}
	return true;
}

void Image::setScale(int width, int height)
{
	GdkPixbuf* orig = 0;
	GdkPixbuf* scaled = 0;

	if(GTK_IMAGE_PIXBUF != gtk_image_get_storage_type(GTK_IMAGE(widget))) {
		printf("Image::setScale() err: Cannot scale without internal pixbuf\n");
		return;
	}

	if(unscaled == NULL) {
		orig = gtk_image_get_pixbuf(GTK_IMAGE(widget));
		g_object_ref(orig);
	}
	else {
		orig = unscaled;
		unscaled = NULL; // it'd be unref'd in setPixbuf() otherwise
	}

	scaled = gdk_pixbuf_scale_simple(orig, width, height, 
				GDK_INTERP_BILINEAR);

	setPixbuf(scaled);
	unscaled = orig; // now we can set
}

void Image::removeScaling()
{
	GdkPixbuf* orig = 0;

	if(unscaled == NULL) {
		return;
	}

	orig = unscaled;
	unscaled = NULL; // it'd be unref'd in setPixbuf() otherwise

	setPixbuf(orig);
}

// XXX NOTE: THIS DOESN'T BELONG HERE
void Image::restoreOriginal()
{
	Cv::Image* orig = 0;
	//orig = getMap("original"); // XXX XXX XXX XXX RESTORE ELSEWHERE
	//setPixbuf(orig->toPixbuf());
}

} // end namespace Gtk
