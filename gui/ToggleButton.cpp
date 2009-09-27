#include "ToggleButton.hpp"

namespace Gtk {

ToggleButton::ToggleButton():
	Widget()
{
	
}

ToggleButton::ToggleButton(std::string label, bool mnemonic):
	Widget()
{
	if(mnemonic) {
		widget =  gtk_toggle_button_new_with_mnemonic(
						(const gchar*)label.c_str());
	}
	else {
		widget = gtk_toggle_button_new_with_label((const gchar*)label.c_str());
	}
}

ToggleButton::ToggleButton(bool a, bool b):
	Widget()
{
	// NOTHING
}

ToggleButton::~ToggleButton()
{
	if(widget != NULL) {
		// TODO
	}
}

bool ToggleButton::getActive()
{
	return (bool)gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
}

void ToggleButton::setActive(bool active)
{
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), active);
}

} // end namespace Gtk
