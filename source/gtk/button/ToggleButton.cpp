#include "ToggleButton.hpp"

namespace Gtk {

ToggleButton::ToggleButton():
	Button(false, false)
{
	
}

ToggleButton::ToggleButton(std::string label, bool mnemonic):
	Button(false, false)
{
	if(mnemonic) {
		widget = gtk_toggle_button_new_with_mnemonic(
						(const gchar*)label.c_str());
	}
	else {
		widget = gtk_toggle_button_new_with_label((const gchar*)label.c_str());
	}
}

ToggleButton::ToggleButton(bool a, bool b):
	Button(false, false)
{
	// NOTHING
}

ToggleButton::~ToggleButton()
{
	// TODO
}

bool ToggleButton::getActive()
{
	return (bool)gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
}

void ToggleButton::setActive(bool active)
{
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), active);
}

void ToggleButton::addToggleCb(void(*userFunc)(GtkToggleButton*, gpointer), 
		gpointer data)
{
	g_signal_connect(widget, "toggled", G_CALLBACK(userFunc), data);
}

} // end namespace Gtk
