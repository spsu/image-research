#ifndef APP_GUI
#define APP_GUI

/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 */

#include <gtk/gtk.h>
#include <string>

namespace Gtk {
	class Window;
	class Widget;
}

namespace App {
class Gui
{
	public:

		/**
		 * Represent a single window at the bare minimum. 
		 * Allow the users to customize the specifics.
		 */
		Gui(std::string title = std::string());

		/**
		 * Destroy the window, quit the app.
		 */
		~Gui();

		/**
		 * Start gui (calls gtk main loop)
		 */
		void start();

		/**
		 * Quit gui (calls gtk_main_quit)
		 */
		void quit();

		/**
		 * Return window pointer.
		 */
		Gtk::Window* getWindow();

		/**
		 * Set child.
		 */
		void setChild(Gtk::Widget* widget);
		void setChild(GtkWidget* widget); // TODO TEMP

		/**
		 * Drag and drop targets.
		 * TODO XXX TEMP: These don't belong here.
		 * Make a gtk/DragDrop class or something to automate adding types. 
		 */
		/*static const unsigned int DND_TARGET_TEXT = 50;
		static const unsigned int DND_TARGET_JPG  = 51;
		static const unsigned int DND_TARGET_PNG  = 52;
		static const unsigned int DND_TARGET_XDIR  = 53;
		static const unsigned int DND_TARGET_INODE  = 54;
		static const unsigned int DND_TARGET_URILIST = 55;
		static const unsigned int DND_TARGET_MOZURL = 56;*/

	protected:
		/**
		 * Resize callback.
		 */		
		//static void checkResizeCb(GtkContainer* container, gpointer data);

		/*
		static gboolean dragMotionCb(GtkWidget* widget, GdkDragContext* dragCtx, 
				gint x, gint y, guint time, gpointer obj);
		static void dragDataGetCb(GtkWidget* widget, GdkDragContext* dragCtx, 
				GtkSelectionData* data, guint info, guint time, gpointer obj);
		static void dragDataReceivedCb(GtkWidget* widget, 
				GdkDragContext* dragCtx, gint x, gint y, GtkSelectionData* data,
				guint info, guint time, gpointer obj);
		*/

	private:
		Gtk::Window* window;

		int curWidth;
		int curHeight;

		
};
}

#endif
