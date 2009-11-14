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

void RgbImage2::destroy(unsigned char* pixels, void* data)
{
	RgbImage2* self = (RgbImage2*)data;
	delete self;
}
