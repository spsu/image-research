#include "CheckButton.hpp"

namespace Gtk {

CheckButton::CheckButton():
	ToggleButton(false, false)
{
	widget = gtk_check_button_new();
}

CheckButton::CheckButton(std::string label, bool mnemonic):
	ToggleButton(false, false)
{
	if(mnemonic) {
		widget =  gtk_check_button_new_with_mnemonic(
						(const gchar*)label.c_str());
	}
	else {
		widget = gtk_check_button_new_with_label((const gchar*)label.c_str());
	}
}

CheckButton::~CheckButton()
{
	if(widget != NULL) {
		// TODO
	}
}

} // end namespace Gtk
