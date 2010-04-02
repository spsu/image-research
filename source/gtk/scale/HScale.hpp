#ifndef BT_Gtk_HScale
#define BT_Gtk_HScale

#include "Scale.hpp"

namespace Gtk {
	class Adjustment;
}

/**
 * Horizontal Scale
 * 		Widget > Scale > HScale
 */
namespace Gtk {
class HScale: public Scale
{
	public:
		/**
		 * HScale adjustment CTOR.
		 */
		HScale(Adjustment* adj);

		/**
		 * HScale primitives (non-adjustment) CTOR
		 * Specify the minimum and maximum values as well as the step value.
		 * Step defaults to 1.0. 
		 */
		HScale(double min, double max, double step = 1.0);
};
}

#endif
