#ifndef GUI_GTK_CHECKBUTTON
#define GUI_GTK_CHECKBUTTON

/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 * 
 * The check button wrapper.
 */

#include "ToggleButton.hpp"
#include <string>

namespace Gtk {
class CheckButton : public ToggleButton
{
	public:
		/**
		 * Create a check button.
		 */
		CheckButton();

		/**
		 * Create a toggle button with a label.
		 * If bool mnemonic, underlines in the label set the mnemonic. 
		 */
		CheckButton(std::string label, bool mnemonic = false);

		/**
		 * Virtual DTOR.
		 */
		virtual ~CheckButton();


};
}

#endif
