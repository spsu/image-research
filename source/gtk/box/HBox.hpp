#ifndef GUI_GTK_HBOX
#define GUI_GTK_HBOX

/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 * 
 * The hbox wrapper.
 */

#include "Box.hpp"
#include <string>

namespace Gtk {
class HBox : public Box
{
	public:
		/**
		 * Create an hbox w/ params.
		 */
		HBox(bool homogeneous = false, int spacing = 0);

		/**
		 * Virtual DTOR.
		 */
		virtual ~HBox();
};
}

#endif
