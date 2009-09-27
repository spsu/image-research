#ifndef GUI_GTK_WIDGET
#define GUI_GTK_WIDGET

/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 * 
 * The gtk widget superclass.
 */

#include <gtk/gtk.h>
#include <string>

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
