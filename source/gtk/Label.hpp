#ifndef GUI_GTK_LABEL
#define GUI_GTK_LABEL

/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 * 
 * The label wrapper.
 */

#include "Widget.hpp"
#include <string>

namespace Gtk {
class Label : public Widget
{
	public:
		/**
		 * Create a label.
		 */
		Label(std::string text = std::string());

		/**
		 * Virtual DTOR.
		 */
		virtual ~Label();

		/**
		 * Set the text.
		 */
		void setText(std::string text);
};
}

#endif
