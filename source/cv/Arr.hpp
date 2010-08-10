#ifndef BT_Cv_Arr
#define BT_Cv_Arr

/**
* Copyright (c) 2010 Brandon Thomas Suit
 * http://possibilistic.org | echelon@gmail.com
 * Code available for use under the LGPL 2.
 * 
 * Cv::Arr
 *   Dummy class (for now) and part of the OpenCV Arr->Mat->Img OO Hierarchy.
 *    TODO: Make uninstantiable? Or is there a need for CvArr?
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
		//CvArr* data;

		/**
		 * If the structure is owned.
		 * In cases where not, it is shared and cannot be deallocated here. 
		 */
		bool isOwner;
		
};
}

#endif
