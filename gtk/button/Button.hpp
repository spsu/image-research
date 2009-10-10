#ifndef GUI_GTK_BUTTON
#define GUI_GTK_BUTTON

/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 * 
 * The button wrapper.
 */

#include "../Widget.hpp"
#include <string>

namespace Gtk {
class Button : public Widget
{
	public:
		/**
		 * Create a button.
		 */
		Button();

		/**
		 * Create a toggle button with a label.
		 * If bool mnemonic, underlines in the label set the mnemonic. 
		 */
		Button(std::string label, bool mnemonic = false);

		/**
		 * Don't construct anything.
		 * TODO: TEST
		 * TODO: Change params to an enum
		 */
		Button(bool a, bool b);

		/**
		 * Virtual DTOR.
		 */
		virtual ~Button();

		/**
		 * Attach a signal handler callback to the "clicked" signal.
		 */
		void addClickedCb(void(*userFunc)(GtkButton*, gpointer), 
				gpointer data);
};
}

#endif
