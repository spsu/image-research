#include "Image.hpp"
#include <stdio.h>

namespace Gui {

Image::Image(): 
	image(0)
{
	image = gtk_image_new();
}

Image::Image(GdkPixbuf* pixbuf):
	image(0)
{
	image = gtk_image_new_from_pixbuf(pixbuf);
}

Image::~Image()
{
	if(image != NULL) {
		gtk_widget_destroy(image);
	}
}

void Image::setPixbuf(GdkPixbuf* pixbuf)
{
	if(pixbuf != NULL) {
		fprintf(stderr, "Image::setPixbuf() err: Pixbuf is null!\n");
		return;
	}
	gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
}

void Image::setFile(std::string filename)
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
		return;
	}

	// Load via pixbuf loader to detect errors
	pixbuf = gdk_pixbuf_new_from_file(filename.c_str(), err);
	if(pixbuf == NULL || err != NULL) {
		fprintf(stderr, "Image::setFile() err: Error loading image.\n");
		return;
	}

	gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
	g_object_unref(pixbuf);
}

GtkWidget* Image::getPtr()
{
	return image;
}

} // end namespace Gui
