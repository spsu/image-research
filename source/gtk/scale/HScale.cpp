#include "HScale.hpp"
#include "../Adjustment.hpp"

namespace Gtk {

HScale::HScale(Adjustment* adj):
	Scale(DIRECTLY_UNINSTANTIABLE)
{
	widget = gtk_hscale_new(GTK_ADJUSTMENT(adj->getPtr()));
	adjustment = adj;
}

HScale::HScale(double min, double max, double step):
	Scale(DIRECTLY_UNINSTANTIABLE)
{
	widget = gtk_hscale_new_with_range(min, max, step);
}

} // end namespace Gtk
