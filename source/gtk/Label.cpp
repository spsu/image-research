#include "Label.hpp"

namespace Gtk {

Label::Label(std::string text):
	Widget()
{
	widget = gtk_label_new((const gchar*)text.c_str());
}

Label::~Label()
{
	// TODO
}

void Label::setText(std::string text)
{
	gtk_label_set_text(GTK_LABEL(widget), (const gchar*)text.c_str());
}

} // end namespace Gtk
