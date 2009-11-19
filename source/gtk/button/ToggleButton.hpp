#ifndef GUI_GTK_TOGGLEBUTTON
#define GUI_GTK_TOGGLEBUTTON

/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 * 
 * The toggle button wrapper.
 */

#include "Button.hpp"
#include <string>

namespace Gtk {
class ToggleButton : public Button
{
	public:
		/**
		 * Create a toggle button.
		 */
		ToggleButton();

		/**
		 * Create a toggle button with a label.
		 * If bool mnemonic, underlines in the label set the mnemonic. 
		 */
		ToggleButton(std::string label, bool mnemonic = false);

		/**
		 * Don't construct anything.
		 * TODO: TEST
		 * TODO: Change params to an enum
		 */
		ToggleButton(bool a, bool b);

		/**
		 * Virtual DTOR.
		 */
		virtual ~ToggleButton();

		/**
		 * Get/Set the state.
		 */
		bool getActive();
		void setActive(bool active = true);

		/**
		 * Attach a signal handler callback to the "toggled" signal.
		 */
		void addToggleCb(void(*userFunc)(GtkToggleButton*, gpointer), 
				gpointer data);
};
}

#endif
