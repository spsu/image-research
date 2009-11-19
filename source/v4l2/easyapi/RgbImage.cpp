#include "RgbImage.hpp"
#include <stdio.h>
#include <stdlib.h>

RgbImage2::RgbImage2(int numBytes)
{
	// XXX FIXME: Allocates extra space to avoid buffer overrun segfaults that
	// Occur when supplying insufficinet numBytes (YUV takes up less space!)
	// the second CTOR is more appropriate/precise. 
	data = (unsigned char*)malloc(numBytes*2);
}

RgbImage2::RgbImage2(int width, int height, int numChannels)
{
	data = (unsigned char*)malloc(width * height * numChannels);
}

RgbImage2::~RgbImage2()
{
	free(data);
}

void RgbImage2::setFromYuyv(const unsigned char* p, int len)
{
	// For YUYV information:
	// http://v4l2spec.bytesex.org/spec/r4339.htm
	// http://www.fourcc.org/fccyvrgb.php
	int j = 0;
	for(int i = 0; i < len; i+=4) 
	{
		int y1 = p[i];
		int cb = p[i+1];
		int y2 = p[i+2];
		int cr = p[i+3];

		//R = 1.164(Y - 16) + 1.596(V - 128)
		//G = 1.164(Y - 16) - 0.813(V - 128) - 0.391(U - 128)
		//B = 1.164(Y - 16)                  + 2.018(U - 128)
		
		int r1 = (int) 1.164*(y1 - 16) + 1.596*(cr - 128);
		int g1 = (int) 1.164*(y1 - 16) - 0.813*(cr - 128) - 0.391*(cb - 128);
		int b1 = (int) 1.164*(y1 - 16) + 2.018*(cb - 128);
		
		int r2 = (int) 1.164*(y2 - 16) + 1.596*(cr - 128);
		int g2 = (int) 1.164*(y2 - 16) - 0.813*(cr - 128) - 0.391*(cb - 128);
		int b2 = (int) 1.164*(y2 - 16) + 2.018*(cb - 128);

		// There has to be a faster way than this... perhaps bitwise?
		if(r1 > 255) {
			r1 = 255;
		}
		if(g1 > 255) {
			g1 = 255;
		}
		if(b1 > 255) {
			b1 = 255;
		}
		if(r2 > 255) {
			r2 = 255;
		}
		if(g2 > 255) {
			g2 = 255;
		}
		if(b2 > 255) {
			b2 = 255;
		}

		if(r1 < 0) {
			r1 = 0;
		}	
		if(g1 < 0) {
			g1 = 0;
		}	
		if(b1 < 0) {
			b1 = 0;
		}	
		if(r2 < 0) {
			r2 = 0;
		}	
		if(g2 < 0) {
			g2 = 0;
		}	 
		if(b2 < 0) {
			b2 = 0;
		}

		data[j] = (int)r1;
		data[j+1] = (int)g1;
		data[j+2] = (int)b1;

		data[j+3] = (int)r2;
		data[j+4] = (int)g2;
		data[j+5] = (int)b2;

		j += 6;
	}
}

void RgbImage2::destroy(unsigned char* pixels, void* data)
{
	RgbImage2* self = (RgbImage2*)data;
	delete self;
}
