#include "SpinButton.hpp"

namespace Gtk {

SpinButton::SpinButton(double min, double max, double step):
	Entry(false, false)
{
	// TODO
	widget = gtk_spin_button_new_with_range(min, max, step);
}

SpinButton::~SpinButton()
{
	// TODO
}

double SpinButton::getValue()
{
	return (double)gtk_spin_button_get_value(GTK_SPIN_BUTTON(widget));
}

void SpinButton::addValueChangedCb(void(*userFunc)(GtkSpinButton*, gpointer),
		gpointer data)
{
	g_signal_connect(widget, "value-changed", G_CALLBACK(userFunc), data);
}

} // end namespace Gtk
