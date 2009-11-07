#ifndef GTK_GTK
#define GTK_GTK

#include <gtk/gtk.h>

namespace Gtk {
	class Widget;
}

namespace Gtk {
class Gtk 
{
	public:
		/**
		 * Init gtk.
		 */
		static void init();

		/**
		 * Start gtk main loop.
		 */
		static void start();

		/**
		 * Quit the gtk main loop / quit application.
		 */
		static void quit();

		/**
		 * Connect a signal.
		 */
		/*static void connect(Widget* widget, std::string sigName, 
							GCallback handler, gpointer data);*/

	protected:
		/**
		 * Cannot instantiate!
		 */
		Gtk();
		Gtk(const Gtk&);
		Gtk& operator=(const Gtk&);

};
}

#endif
