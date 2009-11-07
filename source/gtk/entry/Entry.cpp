#include "Entry.hpp"

namespace Gtk {

Entry::Entry():
	Widget()
{
	widget = gtk_entry_new();
}

Entry::Entry(bool a, bool b):
	Widget()
{
	// NOTHING
}

Entry::~Entry()
{
	// TODO
}

std::string Entry::getText()
{
	std::string ret;
	ret = (const char*)gtk_entry_get_text(GTK_ENTRY(widget));
	return ret;
}

void Entry::setText(std::string text)
{
	gtk_entry_set_text(GTK_ENTRY(widget), (const gchar*)text.c_str());
}

int Entry::getLength()
{
	return (int)gtk_entry_get_text_length(GTK_ENTRY(widget));
}

} // end namespace Gtk
