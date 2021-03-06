#ifndef APP_IMAGEPANE
#define APP_IMAGEPANE

/**
 * Copyright (c) 2009 - 2010 Brandon Thomas Suit
 * http://possibilistic.org | echelon@gmail.com
 * Code available for use under the LGPL 2.
 *
 * ImagePane
 *   Application-specific functionality for "image panes", eg. GtkImage
 *   widgets. This class supports drag/drop, restore-to-original, caching,
 *   etc. for assistance.
 */

#include <string>
#include <gtk/gtk.h> // TODO: TEMP

namespace Cv {
	class Image;
}
namespace App {
	class ImageCache;
}
namespace Gtk {
	class Image;
}

namespace App {
class ImagePane
{
	public:
		/**
		 * ImagePane CTOR.
		 * Optionally specify filename for construction.
		 */
		ImagePane(std::string filename = std::string());

		/**
		 * ImagePane DTOR.
		 * Deletes the GtkImage widget and ImageCache.
		 */
		~ImagePane();

		/**
		 * Get the Gtk::Image widget.
		 */
		Gtk::Image* getImage();

		/**
		 * Get the ImageCache.
		 */
		ImageCache* getCache();

		/**
		 * Drag-drop status.
		 */
		bool acceptsDrag();

		/**
		 * Set drag-drop status.
		 */
		void setAcceptsDrag(bool accept = true);

		/**
		 * Set the original image.
		 * Internal ImageCache becomes the owner.
		 */
		void setOriginal(Cv::Image* img);

		/**
		 * Restore the original image.
		 */
		bool restoreOriginal();

		/**
		 * Drag and drop targets.
		 * TODO XXX TEMP: These don't belong here.
		 * Make a gtk/DragDrop class or something to automate adding types. 
		 */
		static const unsigned int DND_TARGET_TEXT = 50;
		static const unsigned int DND_TARGET_JPG  = 51;
		static const unsigned int DND_TARGET_PNG  = 52;
		static const unsigned int DND_TARGET_XDIR  = 53;
		static const unsigned int DND_TARGET_INODE  = 54;
		static const unsigned int DND_TARGET_URILIST = 55;
		static const unsigned int DND_TARGET_MOZURL = 56;

	protected:
		/**
		 * Drag callback.
		 */
		static gboolean dragMotionCb(GtkWidget* widget, GdkDragContext* dragCtx, 
				gint x, gint y, guint time, gpointer obj);

		/**
		 * Drag callback.
		 */
		static void dragDataGetCb(GtkWidget* widget, GdkDragContext* dragCtx, 
				GtkSelectionData* data, guint info, guint time, gpointer obj);

		/**
		 * Drag callback.
		 */
		static void dragDataReceivedCb(GtkWidget* widget, 
				GdkDragContext* dragCtx, gint x, gint y, GtkSelectionData* data,
				guint info, guint time, gpointer obj);

	private:
		/**
		 * The Gtk Image represented in this pane.
		 */
		Gtk::Image* gtkImage;

		/**
		 * An image cache that may be used to store revision histories for this
		 * ImagePane. Cached images are of type Cv::Image.
		 */
		ImageCache* cache;

		/**
		 * Drag and drop status.
		 */
		bool isAcceptDrag;
};
}

#endif
