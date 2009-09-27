#ifndef GUI_GTK_IMAGE
#define GUI_GTK_IMAGE

/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 * 
 * GtkImage widgets with GdkPixbuf loading. 
 */

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <string>

namespace Gtk {
class Image
{
	public:
		/**
		 * Default CTOR - empty image.
		 */
		Image();

		/**
	 	 * Pixbuf CTOR.
		 */
		Image(GdkPixbuf* pixbuf);

		/**
		 * Image DTOR.
		 */
		~Image();

		/**
		 * Get widget pointer.
		 */
		GtkWidget* getPtr();

		/**
		 * Get the internal pixbuf.
		 * Caller does not own a reference!
		 */
		GdkPixbuf* getPixbuf();

		/**
		 * Set the pixbuf.
		 */
		void setPixbuf(GdkPixbuf* pixbuf);

		/**
		 * Set file.
		 */
		void setFile(std::string filename);

		/**
		 * Set scaling.
		 */
		void setScale(int width, int height);

		/**
		 * Remove any scaling.
		 */
		void removeScaling();

	private:
		/**
		 * The image widget.
		 */
		GtkWidget* image;

		/**
		 * If the image is scaled, the original pixbuf is cached here.
		 */
		GdkPixbuf* unscaled;
};
}

#endif
