#ifndef GUI_GTK_BOX
#define GUI_GTK_BOX

/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 * 
 * The box wrapper.
 */

#include "Widget.hpp"
#include <string>

namespace Gtk {
class Box : public Widget
{
	public:
		/**
		 * Create a box.
		 */
		Box();

		/**
		 * Don't construct anything.
		 * TODO: TEST
		 * TODO: Change params to an enum
		 */
		Box(bool a, bool b);

		/**
		 * Virtual DTOR.
		 */
		virtual ~Box();

		/**
		 * Pack the box.
		 */
		void packStart(Widget* child, bool expand = false, 
					   bool fill = false, int padding = 0);
		void packStart(GtkWidget* child, bool expand = false, 
					   bool fill = false, int padding = 0);
		void packEnd(Widget* child, bool expand = false, 
					   bool fill = false, int padding = 0);
		void packEnd(GtkWidget* child, bool expand = false, 
					   bool fill = false, int padding = 0);
};
}

#endif
