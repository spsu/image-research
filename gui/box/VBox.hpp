#ifndef GUI_GTK_VBOX
#define GUI_GTK_VBOX

/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 * 
 * The vbox wrapper.
 */

#include "Box.hpp"
#include <string>

namespace Gtk {
class VBox : public Box
{
	public:
		/**
		 * Create a vbox w/ params.
		 */
		VBox(bool homogeneous = false, int spacing = 0);

		/**
		 * Virtual DTOR.
		 */
		virtual ~VBox();
};
}

#endif
