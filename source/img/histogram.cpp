#include "histogram.hpp"
#include "../cv/Image.hpp"

#include <stdio.h>
#include <algorithm>

using namespace std;

namespace Img {

Cv::Image* histogram(Cv::Image* img, int width, int height, int padTop, int testIncr)
{
	Cv::Image* hist = 0;
	unsigned int srcWidth, srcHeight;
	unsigned int i, j, k;
	int bv, gv, rv;
	int _max = 0, bmax = 0, gmax = 0, rmax = 0;
	int incr = 0; // pixel increment to compress height 
	incr = testIncr;

	// Histogram bins
	int blue[256] = {0};
	int green[256] = {0};
	int red[256] = {0};

	// XXX/TODO: 
	if(width != 256) {
		fprintf(stderr, "Histogram width must be 256 until I write an interpolation algorithm.\n");
		return 0;
	}
	
	hist = new Cv::Image(width, height); // TODO: Do we need to zero the pixels?

	srcWidth = img->getWidth();
	srcHeight = img->getHeight();

	RgbPix pix = img->getPix(); // TODO: Rename Cv::Pix
	RgbPix out = hist->getPix();

	for(i = 0; i < srcHeight; i++) {
		for(j = 0; j < srcWidth; j++) {
			bv = pix[i][j].b;
			gv = pix[i][j].g;
			rv = pix[i][j].r;

			blue[bv]++;
			green[gv]++;
			red[rv]++;
		}
	}	

	for(int i = 0; i < 256; i++) {
		if(bmax < blue[i])
			bmax = blue[i];
		if(gmax < green[i]) 
			gmax = green[i];
		if(rmax < red[i])
			rmax = red[i];
	}

	_max = max(bmax, max(gmax, rmax));

	printf("Max: %d (%d/%d/%d)\n", _max, bmax, gmax, rmax);

	// pixel increment to compress height 
	if(incr == 0) {
		incr = _max / (height - padTop);
	}
	printf("Increment: %d\n", incr);


	// Build histogram image
	for(i = 0; i < width; i++) {
		bv = blue[i] / incr;
		gv = green[i] / incr;
		rv = red[i] / incr;

		//printf("Bv[%d] = %d   Gv[%d] = %d   Rv[%d] = %d\n", i, bv, i, gv, i, rv);
		printf("b[%d] = %d   g[%d] = %d   r[%d] = %d\n", i, blue[i], i, green[i], i, red[i]);

		for(j = 0; j < height; j++) {
			k = height - 1 - j;
			if(j <= bv) 
				out[k][i].b = 255;
			else
				out[k][i].b = 0;
			if(j <= gv)
				out[k][i].g = 255;
			else
				out[k][i].g = 0;
			if(j <= rv)
				out[k][i].r = 255;
			else
				out[k][i].r = 0;
		}
	}

	return hist;
}

} // end namespace Img
