#include "HBox.hpp"

namespace Gtk {

HBox::HBox(bool homogeneous, int spacing):
	Box(false, false)
{
	widget = gtk_hbox_new(homogeneous, spacing);
}

HBox::~HBox()
{
	// TODO
}

} // end namespace Gtk
