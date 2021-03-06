/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 */

#include "ImagePane.hpp"
#include "ImageCache.hpp"
#include "../gtk/Widget.hpp"
#include "../gtk/Image.hpp"
#include "../cv/Image.hpp"

namespace App {

ImagePane::ImagePane(std::string filename):
	gtkImage(0),
	cache(0),
	isAcceptDrag(true)
{
	Cv::Image* tmpImg = 0;

	// Create members
	if(filename.size() < 1) {
		gtkImage = new Gtk::Image();
	}
	else {
		tmpImg = new Cv::Image(filename); // TODO - check for error.
		gtkImage = new Gtk::Image(tmpImg->toPixbuf());
	}
	cache = new ImageCache();

	// Backup of original 
	if(tmpImg != 0) {
		setOriginal(tmpImg);
	}

	// Enable drag and drop
	setAcceptsDrag(true);

	// Resize signals (TODO: Automate all of these.)
	//g_signal_connect(window->getPtr(), "check-resize", 
	//					G_CALLBACK(checkResizeCb), this);

}

ImagePane::~ImagePane()
{
	delete gtkImage;
	delete cache;
}

Gtk::Image* ImagePane::getImage()
{
	return gtkImage;
}

ImageCache* ImagePane::getCache()
{
	return cache;
}

bool ImagePane::acceptsDrag()
{
	return isAcceptDrag;
}

void ImagePane::setAcceptsDrag(bool accept)
{
	// For drag and drop (TODO: Doesn't belong here)
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

	isAcceptDrag = accept;

	// TODO: Create Drag-Drop class in Gtk/ that automates *ALL* of this.
	// May need to be supplemented by a signal connecting class.
	if(accept) {
		// TODO: Automate all of this
		gtk_drag_dest_set(gtkImage->getPtr(), GTK_DEST_DEFAULT_ALL, dropTypes, 
							numDropTypes, GDK_ACTION_COPY);
		g_signal_connect(gtkImage->getPtr(), "drag-motion", 
							G_CALLBACK(dragMotionCb), this);
		g_signal_connect(gtkImage->getPtr(), "drag-data-get", 
							G_CALLBACK(dragDataGetCb), this);
		g_signal_connect(gtkImage->getPtr(), "drag-data-received", 
							G_CALLBACK(dragDataReceivedCb), this);
	}
	else {
		// TODO: Automate all of this
		/*g_signal_handlers_disconnect_by_func(G_OBJECT(gtkImage->getPtr()), 
							G_CALLBACK(dragMotionCb), this);
		g_signal_handlers_disconnect_by_func(gtkImage->getPtr(), 
							G_CALLBACK(dragDataGetCb), this);
		g_signal_handlers_disconnect_by_func(gtkImage->getPtr(), 
							G_CALLBACK(dragDataReceivedCb), this);*/
		gtk_drag_dest_unset(gtkImage->getPtr());
	}
}

void ImagePane::setOriginal(Cv::Image* img)
{
	cache->del("original");
	cache->set("original", img);
}

bool ImagePane::restoreOriginal()
{
	Cv::Image* orig = 0;

	orig = cache->get("original");
	if(orig == 0) {
		return false;
	}
	gtkImage->setPixbuf(orig->toPixbuf());
	return true;
}

/************************* CALLBACKS ****************************/

gboolean ImagePane::dragMotionCb(GtkWidget* widget, GdkDragContext* dragCtx, 
		gint x, gint y, guint time, gpointer obj)
{
	gdk_drag_status(dragCtx, GDK_ACTION_COPY, time);
	return true;
}

void ImagePane::dragDataGetCb(GtkWidget* widget, GdkDragContext* dragCtx, 
		GtkSelectionData* data, guint info, guint time, gpointer obj)
{
	printf("dragDataGetCb\n");
}

void ImagePane::dragDataReceivedCb(GtkWidget* widget, GdkDragContext* dragCtx,
		gint x, gint y, GtkSelectionData* data, guint type, 
		guint time, gpointer obj)
{
	ImagePane* self = (ImagePane*)obj;
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
		fprintf(stderr, "ImagePane drag and drop err: Filename too short.\n");
		return;
	}	

	// TODO: Do a check to ensure this is a file. Don't try to set HTTP URIs
	// unless I add libcurl or libsoup or something.
	self->gtkImage->setFile(filename);
	//self->cache->del("original");

	// Backup in cache.
	img = new Cv::Image(self->gtkImage->getPixbuf()); // XXX: Potential segfault
	//self->cache->set("original", img);
	self->setOriginal(img);
}

} // end namespace App
