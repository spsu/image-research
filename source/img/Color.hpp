#ifndef BT_IMG_COLOR
#define BT_IMG_COLOR

#include <stdio.h>
#include <stdlib.h>
#include <string>

namespace Img {

using namespace std;

/**
 * Color represents a pixel color in a 3-channel RGB image. 
 * (Values are from 0 to 255.)
 */
struct Color 
{
	int r;
	int g;
	int b;

	/**
	 * CTORs. 
	 */
	Color(): r(0), g(0), b(0) {};
	Color(int _r, int _g, int _b): r(_r), g(_g), b(_b) {};

	/**
	 * Hexidecimal CTOR.
	 */
	Color(std::string hex): r(0), g(0), b(0) {
		string tmp;
		
		if(hex.length() != 3 && hex.length() != 6 && hex.length() != 7 && 
		   hex.length() != 4) {
				fprintf(stderr,	"Color(string) must be hex color code.\n");
				return;
		}

		// TODO: Verify the '3-character hex' follows the real spec.
		switch(hex.length()) {
			case 3:
				// XXX: Ugh, string concat must be better
				hex = string(2, hex[0]) + string(2, hex[1]) + string(2, hex[2]);
				break;
			case 4:
				// Ignores first character, assumed to be '#'
				hex = string(2, hex[1]) + string(2, hex[2]) + string(2, hex[3]);
				break;
			case 7:
				// Ignores first character, assumed to be '#'
				hex = hex.substr(1, 6);
				break;
		}

		r = (int)strtol(hex.substr(0, 2).c_str(), 0, 16);
		g = (int)strtol(hex.substr(2, 2).c_str(), 0, 16);
		b = (int)strtol(hex.substr(4, 2).c_str(), 0, 16);
	};
};

} // end namespace Img

#endif
