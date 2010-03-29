#ifndef BT_Cv_Mat
#define BT_Cv_Mat

#include "Arr.hpp"
#include <cv.h>

namespace Cv {
class Mat : public Arr
{

	public:
		/**
		 * Default CTOR.
		 * Does absolutely nothing.
		 */
		Mat();

		/**
		 * CTOR.
		 */
		Mat(int rows, int cols, int type);

		/**
		 * Copy CTOR.
		 */
		Mat(const Mat& m);

		/**
		 * Init CTOR.
		 */
		//Mat(int rows, int cols, std::vector<*> d); TODO

		/**
		 * DTOR.
		 */
		virtual ~Mat();

		/**
		 * Get ptr.
		 * XXX: This could be problematic, since it can't be virtual...
		 */
		CvMat* getPtr() { return mat; };

		/**
		 * Get size of the matrix.
		 */
		int getWidth();
		int getHeight();
		CvSize getSize();

	protected:
		/**
		 * Matrix and Image structs, though only one may be used per object.
		 * Unfortunately, although OpenCV allows for polymorphic operations on
		 * its data structures, that's difficult to implement without Gtk-like
		 * macros to switch between them. 
		 */
		CvMat* mat;
		IplImage* image;

};
}

#endif
