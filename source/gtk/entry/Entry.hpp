#ifndef GUI_GTK_ENTRY
#define GUI_GTK_ENTRY

/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 * 
 * The entry wrapper.
 */

#include "../Widget.hpp"
#include <string>

namespace Gtk {
class Entry : public Widget
{
	public:
		/**
		 * Create an entry.
		 */
		Entry();

		/**
		 * Don't construct anything.
		 * TODO: TEST
		 * TODO: Change params to an enum
		 */
		Entry(bool a, bool b);

		/**
		 * Virtual DTOR.
		 */
		virtual ~Entry();

		/**
		 * Get/Set the text.
		 */
		std::string getText();
		void setText(std::string text);
		
		/**
		 * Get the length of the text.
		 */
		int getLength();

		/**
		 * Attach a signal handler callback to the "toggled" signal.
		 */
		void addToggleCb(void(*userFunc)(GtkToggleButton*, gpointer), 
				gpointer data);
};
}

#endif
