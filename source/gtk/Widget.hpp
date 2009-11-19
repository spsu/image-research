#ifndef GUI_GTK_WIDGET
#define GUI_GTK_WIDGET

/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 */

#include <gtk/gtk.h>
#include <string>

/**
 * The Gtk widget superclass.
 * All other classes in my wrapper inherit from this, with the exception of
 * the Gtk class.
 */
namespace Gtk {
class Widget
{
	public:
		Widget();

		virtual ~Widget();

		/**
		 * Get pointer.
		 */
		GtkWidget* getPtr();

		/**
		 * Widget visibility
		 */
		void show();
		void showAll();
		void hide();
		void hideAll();

	protected:
		/**
		 * The widget.
		 */
		GtkWidget* widget;
};
}

#endif
