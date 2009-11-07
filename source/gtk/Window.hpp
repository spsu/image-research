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

#include "Widget.hpp"
#include <string>
#include <utility>

namespace Gtk {
class Window : public Widget
{
	public:
		/**
		 * Create a window. 
		 * Optionally specify title.
		 */
		Window(std::string title = std::string());

		/**
		 * Window DTOR.
		 */
		~Window();

		/**
		 * Set title.
		 */
		void setTitle(std::string title);

		/**
		 * Add child.
		 * TODO: Conflict resolution for already-set children
		 */
		void setChild(Widget* child);
		void setChild(GtkWidget* child); // TODO: Necessary?

		/**
		 * Get size.
		 * pair.first=width, pair.second=height.
		 * XXX: Can lead to race condition. See Gtk docs.
		 */
		std::pair<int, int> getSize();
};
}

#endif
