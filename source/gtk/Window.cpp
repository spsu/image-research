/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 */

#include "Window.hpp"
#include <stdio.h>

namespace Gtk {

Window::Window(std::string title):
	Widget()
{
	widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	if(title.size() > 0) {
		gtk_window_set_title(GTK_WINDOW(widget), title.c_str());
	}
}

Window::~Window()
{
	// TODO
}

void Window::setTitle(std::string title)
{
	gtk_window_set_title(GTK_WINDOW(widget), title.c_str());
}

void Window::setChild(Widget* child)
{
	gtk_container_add(GTK_CONTAINER(widget), (GtkWidget*)(child->getPtr()));
}

void Window::setChild(GtkWidget* child)
{
	gtk_container_add(GTK_CONTAINER(widget), child);
}

std::pair<int, int> Window::getSize()
{
	std::pair<int, int> size; // width, height.
	gtk_window_get_size(GTK_WINDOW(widget), &size.first, &size.second);
	return size;
}

} // end namespace Gtk
