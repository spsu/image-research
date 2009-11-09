#ifndef GUI_GTK_IMAGE
#define GUI_GTK_IMAGE

/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 */

#include "Widget.hpp"
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <string>
#include <map>

namespace Cv {
	class Image;
}

/**
 * GtkImage widgets with GdkPixbuf loading. 
 */
namespace Gtk {
class Image : public Widget
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
		 * Returns false on failure.
		 */
		bool setFile(std::string filename);
};
}

#endif
