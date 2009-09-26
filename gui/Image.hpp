#ifndef GUI_IMAGE
#define GUI_IMAGE

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

namespace Gui {
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
		 * Set the pixbuf.
		 */
		void setPixbuf(GdkPixbuf* pixbuf);

		/**
		 * Set file.
		 */
		void setFile(std::string filename);

	private:
		/**
		 * The image widget.
		 */
		GtkWidget* image;
};
}

#endif
