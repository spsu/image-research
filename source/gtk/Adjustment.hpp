#ifndef BT_Gtk_Adjustment
#define BT_Gtk_Adjustment

#include <gtk/gtk.h>

/**
 * Represents an adjustable value as used in many Gtk functions/widgets.
 * XXX: Currently this is only implemented in Scale [HScale/VScale]
 * TODO: Implement in SpinButton.
 * XXX/TODO: NOT ALL METHODS IMPLEMENTED!!!
 */
namespace Gtk {
class Adjustment
{
	public:
		/**
		 * Adjustment CTOR.
		 * Creates and adjustment with the specified range and sizes. 
		 * XXX NOTE: Page nor PageSize are supported by this class. (I believe 
		 * that their use is in fact deprecated.)
		 */
		Adjustment(double init, double min, double max, double step = 1.0);

		/**
		 * Wrapper CTOR.
		 * Wrap an adjustment obtained from an owner widget.
		 * Clients won't call this typically.
		 */
		Adjustment(GtkAdjustment* adj);

		/**
		 * Adjustment DTOR.
		 */
		~Adjustment();

		/**
		 * Get the pointer.
		 */
		GtkObject* getPtr();

		/**
		 * Emit the "changed" signal.
		 * Typically called by the owner of the adjustment if any non-value 
		 * fields of the adjustment are changed.
		 */
		void changed();

		/**
		 * Emit the "value_changed" signal.
		 * Typically called by the owner of the adjustment if the value is 
		 * changed. 
		 */
		void valueChanged();

		/**
		 * Get the value.
		 */
		double getValue();

		/**
		 * Set the value.
		 */
		void setValue(double val); 

		/**
		 * Access the lower bounds.
		 */
		double getLower();
		void setLower(double lower);

		/**
		 * Access the upper bounds.
		 */
		double getUpper();
		void setUpper(double upper);

		/**
		 * Access the step increment.
		 * XXX: Doesn't conform to Gtk method names *_step_increment()
		 */
		double getStep();
		void setStep(double step);
		
	protected:
		/**
		 * The adjustment object.
		 */
		GtkObject* adjust;
};
}

#endif
