#ifndef BT_Gtk_VScale
#define BT_Gtk_VScale

#include "Scale.hpp"

namespace Gtk {
	class Adjustment;
}

/**
 * Vertical Scale
 * 		Widget > Scale > VScale
 */
namespace Gtk {
class VScale: public Scale
{
	public:
		/**
		 * VScale adjustment CTOR.
		 */
		VScale(Adjustment* adj);

		/**
		 * VScale primitives (non-adjustment) CTOR
		 * Specify the minimum and maximum values as well as the step value.
		 * Step defaults to 1.0. 
		 */
		VScale(double min, double max, double step = 1.0);
};
}

#endif
