#include "Button.hpp"

namespace Gtk {

Button::Button():
	Widget()
{
	widget = gtk_button_new();
}

Button::Button(std::string label, bool mnemonic):
	Widget()
{
	if(mnemonic) {
		widget = gtk_button_new_with_mnemonic((const gchar*)label.c_str());
	}
	else {
		widget = gtk_button_new_with_label((const gchar*)label.c_str());
	}
}

Button::Button(bool a, bool b):
	Widget()
{
	// NOTHING
}

Button::~Button()
{
	// TODO
}

std::string Button::getLabel()
{
	std::string temp;
	temp = gtk_button_get_label(GTK_BUTTON(widget));
	return temp;
}

void Button::setLabel(std::string label)
{
	gtk_button_set_label(GTK_BUTTON(widget), (const gchar*)label.c_str());
}

void Button::addClickedCb(void(*userFunc)(GtkButton*, gpointer), 
		gpointer data)
{
	g_signal_connect(widget, "clicked", G_CALLBACK(userFunc), data);
}

} // end namespace Gtk
