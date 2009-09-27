#ifndef GUI_GTK_WINDOW
#define GUI_GTK_WINDOW

/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 * 
 * The main gtk window. (Fairly rigid and unflexible class, however.)
 */

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <string>

namespace Gtk {
	class Image;
}

namespace Gtk {
class Window
{
	public:
		/**
		 * Create a window with a title.
		 */
		Window(std::string title);

		/**
		 * Window DTOR.
		 * Kills Gtk.
		 */
		~Window();

		/**
		 * Start main loop.
		 */
		void start();

		/**
		 * Set title.
		 */
		void setTitle(std::string title);

		/**
		 * Get image so that we can work with it.
		 * Window does nothing to the image except display it.
		 */
		Image* getImage();

		// TODO TEST
		static void resizeToggleCb(GtkToggleButton* button, gpointer data);

		/**
		 * Drag and drop targets.
		 */
		static const unsigned int DND_TARGET_TEXT = 50;
		static const unsigned int DND_TARGET_JPG  = 51;
		static const unsigned int DND_TARGET_PNG  = 52;
		static const unsigned int DND_TARGET_XDIR  = 53;
		static const unsigned int DND_TARGET_INODE  = 54;
		static const unsigned int DND_TARGET_URILIST = 55;
		static const unsigned int DND_TARGET_MOZURL = 56;

	protected:
		static gboolean dragMotionCb(GtkWidget* widget, GdkDragContext* dragCtx, 
				gint x, gint y, guint time, gpointer obj);

		static void dragDataGetCb(GtkWidget* widget, GdkDragContext* dragCtx, 
				GtkSelectionData* data, guint info, guint time, gpointer obj);

		static void dragDataReceivedCb(GtkWidget* widget, 
				GdkDragContext* dragCtx, gint x, gint y, GtkSelectionData* data,
				guint info, guint time, gpointer obj);

	private:
		/**
		 * Main window widget.
		 */
		GtkWidget* window;

		/**
		 * Image editing pane.
		 */
		Image* image;
};
}

#endif
