#ifndef CV_HISTOGRAM
#define CV_HISTOGRAM

/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 */

#include <cv.h>
#include <highgui.h>

namespace Cv {
	class Image;
}

namespace Cv {
class Histogram
{
	public:
		/**
		 * Histogram CTOR
		 * XXX: Only supports 1 dimension for now.
		 */
		Histogram(int hSize = 256, int hDimensions = 1);

		/**
		 * DTOR.
		 */
		~Histogram();

		/**
		 * Clears the histogram bins.
		 */
		void clear();

		/**
		 * Query the value of a histogram bin.
		 * XXX: Only supports 1 to 3 dimensions for now.
		 */
		float query(int x, int y = -1, int z = -1);

		/**
		 * Calculate histogram for one image.
		 * TODO: Support multiple images.
		 * TODO: Support mask parameter.
		 */
		void calculate(Image* image, bool accumulate = false);

	protected:
		/**
		 * Histogram
		 */
		CvHistogram* histogram;

		/**
		 * Histogram size
		 */
		int size;

		/**
		 * Number of histogram dimensions.
		 */
		int dimensions;
};
}

#endif
