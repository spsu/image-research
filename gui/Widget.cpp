#include "Widget.hpp"

namespace Gtk {

Widget::Widget():
	widget(0)
{
	// Nothing
}

Widget::~Widget()
{
	// Nothing
}

GtkWidget* Widget::getPtr()
{
	return widget;
}

void Widget::show()
{
	gtk_widget_show(widget);
}

void Widget::showAll()
{
	gtk_widget_show_all(widget);
}

void Widget::hide()
{
	gtk_widget_hide(widget);
}

void Widget::hideAll()
{
	gtk_widget_hide_all(widget);
}

} // end namespace Gtk
