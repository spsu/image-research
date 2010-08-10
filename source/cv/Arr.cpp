/**
 * Copyright (c) 2010 Brandon Thomas Suit
 * http://possibilistic.org | echelon@gmail.com
 * Code available for use under the LGPL 2.
 * 
 * Cv::Arr
 *   Dummy class (for now) and part of the OpenCV Arr->Mat->Img OO Hierarchy.
 *    TODO: Make uninstantiable? Or is there a need for CvArr?
 */

#include "Arr.hpp"

namespace Cv {

Arr::Arr():
	//data(0),
	isOwner(true)
{
	// Nothing
}

} // end namespace Cv
