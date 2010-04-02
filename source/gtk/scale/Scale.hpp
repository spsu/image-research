#ifndef BT_Gtk_Scale
#define BT_Gtk_Scale

#include "../Widget.hpp"
#include "../types.hpp"
#include <string>

namespace Gtk {
	class Adjustment;
}

/**
 * Scale is an interface representing GtkScale, which itself is a base class 
 * that cannot be directly instantiated. Must create an HScale or a VScale 
 * instance.
 * XXX XXX: SHOULD NOT DIRECTLY INSTANTIATE!! XXX XXX
 * TODO: Represent full hierarchy by adding "Range":
 *			Widget > Range > Scale > ... 
 *
 *		Usage (with HScale or VScale): 
 *			// TODO: Change adjustment params to be more in line with *Scale,
 *			// albeit at a cost to non-Gtk conformance
 *			adjust = new Adjustment(0.0, 0.0, 100.0); 
 *			hscale = new HScale(adjust);
 *					OR
 *			hscale = new HScale(0.0, 100.0); // step defaults to 1.0
 *			
 *			hscale.setDigits(0); // No decimal part
 *			hscale.addMark(25.0, POS_TOP, "Q1"); // Mark positions
 *			hscale.addMark(50.0, POS_TOP, "M"); 
 *			hscale.addMark(75.0, POS_TOP, "Q3");
 */
namespace Gtk {
class Scale : public Widget
{
	public:
		/**
		 * UNINSTANTIABLE CTOR.
		 * XXX: You cannot instantitate this type directly.
		 */
		Scale(UninstantiableType u);

		/**
		 * Scale DTOR.
		 * Destroys the Adjustment object if a Scale subclass created it.
		 */
		virtual ~Scale();

		/**
		 * Gets the number of digits to round to.
		 */
		int getDigits();

		/**
		 * Sets the number of digits to round to.
		 */
		void setDigits(int digits);

		/**
		 * Whether the current scale value is displayed as a string next to the
		 * scale itself.
		 */
		bool getDrawValue();

		/**
		 * Set whether the current scale value is displayed as a string next to 
		 * the scale itself.
		 */
		void setDrawValue(bool doDraw = true);

		/**
		 * Get the position where the value is displayed.
		 */
		PositionType getValuePos();

		/**
		 * Set the position where the value is displayed.
		 */
		void setValuePos(PositionType pos);

		/**
		 * Add a marker on the scale.
		 * The marker will be created at the position indicated with optional
		 * text provided. 
		 */
		void addMark(double val, PositionType pos, std::string text = "");

		/**
		 * Remove all markers created from the scale.
		 */
		void clearMarks();

		// === XXX: THESE METHODS BELONG IN "RANGE" CLASS, THE SUPERCLASS === //

		/**
		 * Access the GtkAdjustment via a wrapper class.
		 * Caller does not own the object returned.
		 */
		Adjustment* getAdjustment();

		/**
		 * Access the adjustment's value
		 * XXX: Should be in a "RANGE" superclass.
		 */
		double getValue();
		void setValue(double val);

		/**
		 * Set the minimum and maxium values. 
		 * XXX: Should be in a "RANGE" superclass.
		 */
		void setRange(double min, double max);

		/**
		 * Attach a signal handler callback to the "value-changed" signal.
		 */
		void addValueChangedCb(void(*userFunc)(GtkRange*, gpointer));

	protected:

		/**
		 * This is the adjustment held by the scale.
		 * If accessed, it should not be deleted. 
		 * XXX: This should really be in the "RANGE" Superclass.
		 */
		Adjustment* adjustment;

		/**
		 * Whether the adjustment was created in this class.
		 */
		bool adjustmentCreated;

};
}

#endif
