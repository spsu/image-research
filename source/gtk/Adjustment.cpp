#include "Adjustment.hpp"
#include <stdio.h>

namespace Gtk {

Adjustment::Adjustment(double init, double min, double max, double step):
	adjust(0)
{
	adjust = gtk_adjustment_new(init, min, max, step, 0.0, 0.0);
}

Adjustment::Adjustment(GtkAdjustment* adj)
{
	// XXX: Verify that DTOR won't segfault if this and the owner are deleted
	adjust = GTK_OBJECT(adj);
	g_object_ref(adjust);
}

Adjustment::~Adjustment()
{
	gtk_object_destroy(adjust); // Notifies all owners to release. 
}

GtkObject* Adjustment::getPtr()
{
	return adjust;
}

double Adjustment::getValue()
{
	return gtk_adjustment_get_value(GTK_ADJUSTMENT(adjust));
}

void Adjustment::setValue(double val)
{
	gtk_adjustment_set_value(GTK_ADJUSTMENT(adjust), val);
}

void Adjustment::changed()
{
	gtk_adjustment_changed(GTK_ADJUSTMENT(adjust));
}

void Adjustment::valueChanged()
{
	gtk_adjustment_value_changed(GTK_ADJUSTMENT(adjust));
}

double Adjustment::getLower()
{
	return gtk_adjustment_get_lower(GTK_ADJUSTMENT(adjust));
}

void Adjustment::setLower(double lower)
{
	gtk_adjustment_set_lower(GTK_ADJUSTMENT(adjust), lower);
}

double Adjustment::getUpper()
{
	return gtk_adjustment_get_upper(GTK_ADJUSTMENT(adjust));
}

void Adjustment::setUpper(double upper)
{
	gtk_adjustment_set_upper(GTK_ADJUSTMENT(adjust), upper);
}

double Adjustment::getStep()
{
	return gtk_adjustment_get_step_increment(GTK_ADJUSTMENT(adjust));
}

void Adjustment::setStep(double step)
{
	gtk_adjustment_set_step_increment(GTK_ADJUSTMENT(adjust), step);
}

} // end namespace Gtk
