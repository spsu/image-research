#include "VBox.hpp"

namespace Gtk {

VBox::VBox(bool homogeneous, int spacing):
	Box(false, false)
{
	widget = gtk_vbox_new(homogeneous, spacing);
}

VBox::~VBox()
{
	// TODO
}

} // end namespace Gtk
