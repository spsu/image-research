#include "Box.hpp"

namespace Gtk {

Box::Box():
	Widget()
{
	
}

Box::Box(bool a, bool b):
	Widget()
{
	// NOTHING
}

Box::~Box()
{
	if(widget != NULL) {
		// TODO
	}
}

void Box::packStart(Widget* child, bool expand, bool fill, int padding)
{
	gtk_box_pack_start(GTK_BOX(widget), child->getPtr(), expand, fill, padding);
}

void Box::packStart(GtkWidget* child, bool expand, bool fill, int padding)
{
	gtk_box_pack_start(GTK_BOX(widget), child, expand, fill, padding);
}

void Box::packEnd(Widget* child, bool expand, bool fill, int padding)
{
	gtk_box_pack_end(GTK_BOX(widget), child->getPtr(), expand, fill, padding);
}

void Box::packEnd(GtkWidget* child, bool expand, bool fill, int padding)
{
	gtk_box_pack_end(GTK_BOX(widget), child, expand, fill, padding);
}

} // end namespace Gtk
