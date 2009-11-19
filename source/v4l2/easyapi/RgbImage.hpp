#ifndef NoNamespace_TODO_RgbImage
#define NoNamespace_TODO_RgbImage

#include <gdk-pixbuf/gdk-pixbuf.h>

/**
 * XXX XXX XXX THIS DOESN'T BELONG TO V4L2 XXX XXX XXX 
 * I've only put this here temporarily so that Gtk (or whatever) can do closures
 * on malloc'd data.
 */
struct RgbImage2
{
	/**
	 * Pixel data
	 */
	unsigned char* data;

	/**
	 * Length of data in bytes.
	 */
	int length;

	/**
	 * CTOR to malloc
	 * XXX FIXME: Allocates extra space to avoid buffer overrun segfaults that
	 * Occur when supplying insufficinet numBytes (YUV takes up less space!)
	 * the second CTOR is more appropriate/precise. 
	 */
	RgbImage2(int numBytes);

	/**
	 * USE THIS CTOR!
	 */
	RgbImage2(int width, int height, int numChannels = 3);

	/**
	 * DTOR to free
	 */
	~RgbImage2();

	/**
	 * Set RGB data from conversion of YUYV data supplied
	 */
	void setFromYuyv(const unsigned char* p, int len);

	/**
	 * Closure that auto-destructs (For use with Gtk/GdkPixbuf).
	 */
	static void destroy(unsigned char* pixels, void* data);

};

#endif
