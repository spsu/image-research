#ifndef BT_Cv_Arr
#define BT_Cv_Arr

/**
 * This OO hierarchy is somewhat inefficient, but it will save development time.
 */

#include <cv.h>

namespace Cv {
class Arr
{
	public:
		/**
		 * Nothing.
		 */
		Arr();

	protected:
		/**
		 * Data.
		 */
		CvArr* data;

		/**
		 * If the structure is owned.
		 * In cases where not, it is shared and cannot be deallocated here. 
		 */
		bool isOwner;
		
};
}

#endif
