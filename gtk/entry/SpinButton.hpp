#ifndef GUI_GTK_SPINBUTTON
#define GUI_GTK_SPINBUTTON

/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 * 
 * The spin button wrapper.
 */

#include "Entry.hpp"
#include <string>

namespace Gtk {
class SpinButton : public Entry
{
	public:
		/**
		 * Create a spin button.
		 */
		SpinButton(double min, double max, double step);

		/**
		 * Virtual DTOR.
		 */
		virtual ~SpinButton();

		/**
		 * Get the value.
		 */
		double getValue();

		/**
		 * Attach a signal handler callback to the "value-changed" signal.
		 */
		void addValueChangedCb(void(*userFunc)(GtkSpinButton*, gpointer), 
				gpointer data);
};
}

#endif
