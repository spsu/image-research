#include "Scale.hpp"
#include "../Adjustment.hpp"
#include <stdio.h>

namespace Gtk {

Scale::Scale(UninstantiableType u):
	Widget(),
	adjustment(0),
	adjustmentCreated(false)
{
	// Nothing, uninstantiable directly. 
}

Scale::~Scale()
{
	if(adjustmentCreated) {
		delete adjustment;
	}
}

int Scale::getDigits()
{
	return gtk_scale_get_digits(GTK_SCALE(widget));
}

void Scale::setDigits(int digits)
{
	return gtk_scale_set_digits(GTK_SCALE(widget), digits);
}

bool Scale::getDrawValue()
{
	return gtk_scale_get_draw_value(GTK_SCALE(widget));
}

void Scale::setDrawValue(bool draw)
{
	return gtk_scale_set_draw_value(GTK_SCALE(widget), draw);
}

PositionType Scale::getValuePos()
{
	switch(gtk_scale_get_value_pos(GTK_SCALE(widget))) {
		case GTK_POS_LEFT: 
			return POS_LEFT;
		case GTK_POS_RIGHT:
			return POS_RIGHT;
		case GTK_POS_TOP:
			return POS_TOP;
		case GTK_POS_BOTTOM:
			return POS_BOTTOM;
	}
	fprintf(stderr, "PositionType is incorrect\n");
	return POS_LEFT; // TODO: Error cond. 
}

void Scale::setValuePos(PositionType pos)
{
	GtkPositionType p;

	switch(pos) {
		case POS_LEFT:
			p = GTK_POS_LEFT;
			break;
		case POS_RIGHT:
			p = GTK_POS_RIGHT;
			break;
		case POS_TOP:
			p = GTK_POS_TOP;
			break;
		case POS_BOTTOM:
			p = GTK_POS_BOTTOM;
			break;
		default:
			fprintf(stderr, "PositionType is incorrect\n");
			p = GTK_POS_LEFT; // TODO: Error cond.
	}

	gtk_scale_set_value_pos(GTK_SCALE(widget), p);
}

void Scale::addMark(double val, PositionType pos, std::string text)
{
	GtkPositionType p;

	switch(pos) {
		case POS_LEFT:
			p = GTK_POS_LEFT;
			break;
		case POS_RIGHT:
			p = GTK_POS_RIGHT;
			break;
		case POS_TOP:
			p = GTK_POS_TOP;
			break;
		case POS_BOTTOM:
			p = GTK_POS_BOTTOM;
			break;
		default:
			fprintf(stderr, "PositionType is incorrect\n");
			p = GTK_POS_LEFT; // TODO: Error cond.
	}

	gtk_scale_add_mark(GTK_SCALE(widget), val, p, text.c_str());
}

void Scale::clearMarks() 
{
	gtk_scale_clear_marks(GTK_SCALE(widget));
}

Adjustment* Scale::getAdjustment()
{
	GtkAdjustment* adjust = 0;

	if(adjustment == NULL) {
		adjust = gtk_range_get_adjustment(GTK_RANGE(widget));
		adjustment = new Adjustment(adjust);
		adjustmentCreated = true;
	}
	return adjustment;
}

double Scale::getValue()
{
	return gtk_range_get_value(GTK_RANGE(adjustment));
}

void Scale::setValue(double val)
{
	gtk_range_set_value(GTK_RANGE(adjustment), val);
}

void Scale::setRange(double min, double max)
{
	gtk_range_set_range(GTK_RANGE(adjustment), min, max);
}

void Scale::addValueChangedCb(void(*userFunc)(GtkRange*, gpointer), 
				gpointer data)
{
	g_signal_connect(widget, "value-changed", G_CALLBACK(userFunc), data);
}

} // end namespace Gtk
