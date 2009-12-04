#include "VScale.hpp"
#include "../Adjustment.hpp"

namespace Gtk {

VScale::VScale(Adjustment* adj):
	Scale(DIRECTLY_UNINSTANTIABLE)
{
	widget = gtk_vscale_new(GTK_ADJUSTMENT(adj->getPtr()));
	adjustment = adj;
}

VScale::VScale(double min, double max, double step):
	Scale(DIRECTLY_UNINSTANTIABLE)
{
	widget = gtk_vscale_new_with_range(min, max, step);
}

} // end namespace Gtk
