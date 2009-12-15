#ifndef BT_Cv_PerspectiveTransform
#define BT_Cv_PerspectiveTransform

#include "types.hpp"
#include <cv.h>

namespace Cv {
	class Image;
}

/**
 * Make Perspective Transforms slightly easier. 
 * XXX: THIS IS A WORK IN PROGRESS! CALCULATIONS CANNOT BE TRUSTED XXX.
 * TODO: Clipping needs work. 
 */
namespace Cv {
class PerspectiveTransform
{
	public:

		/**
		 * CTOR.
		 * Supply the size of the image. 
		 */
		PerspectiveTransform(int width, int height);

		/**
		 * CTOR #2.
		 * Supply the size of the image. 
		 */
		PerspectiveTransform(Size sz);

		/**
		 * DTOR.
		 */
		~PerspectiveTransform();

		/**
		 * Reset source and destination points to their default. 
		 * Does not affect translation; call resetTranslation() for that.
		 */
		void reset();

		/**
		 * Manually set the source points.
		 * This shouldn't be required. 
		 */
		void setSrc(int i, float x, float y);

		/**
		 * Manually set the destination points.
		 */
		void setDst(int i, float x, float y);

		/**
		 * Set translation in the destination image.
		 */
		void setTranslation(float x, float y);
		void unsetTranslation();

		/**
		 * Set the "no clipping" boolean. 
		 * If asserted, negative image coordinates are shifted back inside 
		 * positive-only ranges. This helps ensure that a warped image can be 
		 * kept entirely within the output image.
		 * XXX: Doesn't take into account negative translation. 
		 */
		void setNoClipping(bool set = true) { isNoClipping = set; };
		bool isClipping() { return !isNoClipping; };

		/**
		 * Set field of view / depth of view.
		 * XXX: Not sure if this is what it should be called. Only used in 
		 * rotation about X or Y axis.
		 */
		void setFov(float f) { fov = f; };

		/**
		 * Get the minimum size of the destination image that would entirely 
		 * contain the warped source image.
		 */
		Size getClippedSize();

		/**
		 * Set the rotation in *degrees* along each axis. 
		 * If there is a point set, that is the rotation point. 
		 */
		void setRotationX(float deg);
		void setRotationY(float deg);
		void setRotationZ(float deg, int x = 0, int y = 0);

		// TODO: setRotationLine(EQUATION_OF_LINE, float deg);
		// TODO: setRotationAroundPoint(int x, int y, float deg);

		/**
		 * Print the matrix.
		 * For debugging purposes. 
		 */
		void printMat();

		/**
		 * Get a shared pointer to the matrix.
		 * DO NOT DEALLOCATE.
		 */
		CvMat* getMat() { return mat; };

		/**
		 * Update transformation matrix. 
		 * Needs to be called every time rotation is changed, although it is 
		 * automatically called (if needed) by warpPerspective(). 
		 */
		void updateMat();

		/**
		 * Warp image perspective. 
		 * Automatically updates matrix before performing.
		 */
		void warpPerspective(Image* src, Image* dst);

	private:
		/**
		 * Dimensions of the image.
		 */
		int width;
		int height;

		/**
		 * Translation in the output image.
		 * Origin (0, 0) is at top left corner of the destination image. 
		 */
		float xTranslation;
		float yTranslation;

		/**
		 * Matrix
		 */
		CvMat* mat;

		/**
		 * Whether matrix is up to date with transforms. 
		 */
		bool isMatStale;

		/**
 		 * Source and destination points
 		 */
		CvPoint2D32f src[4];
		CvPoint2D32f dst[4];

		/**
		 * Field of view.
		 */
		float fov;

		/**
		 * Whether or not to prevent clipping.
		 * If asserted, shifts negative values back into frame. 
		 */
		bool isNoClipping;

		/**
		 * PI = 3.14159265358979323846
		 */
		const static double PI = 3.14159265358979323846;

		/**
		 * Convert degrees to radians.
		 * (Not in math.h, hence inclusion here.
		 */
		double deg2rad(double deg) { return deg * PI / 180; };

};
}

#endif
