#include "threshold.hpp"
#include "grayscale.hpp"
#include "../cv/Image.hpp"
#include <cv.h>
#include <stdlib.h>

namespace Img {

// XXX/TODO
void threshold(Cv::Image* img, int numRegions)
{
}

void binaryThreshold(Cv::Image* img)
{
	int width, height, area;
	unsigned long sum;
	int avg;

	// Must operate on grayscale image
	grayscale(img);

	width = img->getWidth();
	height = img->getHeight();

	area = width*height;

	RgbPix pix = img->getPix(); // TODO: Rename Cv::Pix

	sum = 0;
	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			sum += pix[i][j].r;
		}
	}

	avg = sum / area;
	
	int thresh = avg;
	
	bool stable = false;

	int avg1 = 0;
	int avg2 = 0;
	int avg1Old = 0;
	int avg2Old = 0;

	while(!stable) {
		int cnt1 = 0;
		int cnt2 = 0;
		long sum1 = 0;
		long sum2 = 0;
		
		avg1Old = avg1;
		avg2Old = avg2;

		// Sum the regions 
		for(int i = 0; i < height; i++) {
			for(int j = 0; j < width; j++) {
				int p = pix[i][j].r;
				if (p <= thresh) {
					sum1 += p;
					cnt1++;
				}
				else {
					sum2 += p;
					cnt2++;
				}
			}
		}

		avg1 = sum1 / cnt1;
		avg2 = sum2 / cnt2;

		// Iterations continue until avg1 and avg2 stablize
		if(avg1 == avg1Old && avg2 == avg2Old) {
			stable = true;
		}

		thresh = (avg1 + avg2) / 2;
		printf("Threshold is: %d\n", thresh);
	}

	// Apply threshold 
	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			if (thresh <= pix[i][j].r) {
				pix[i][j].r = 0;
				pix[i][j].g = 0;
				pix[i][j].b = 0;
			}
			else {
				pix[i][j].r = 255;
				pix[i][j].g = 255;
				pix[i][j].b = 255;
			}	
		}
	}
}

// XXX TODO
Cv::Image* thresholdCopy(Cv::Image* img, int numRegions)
{
	return 0;
}

// XXX TODO
Cv::Image* binaryThresholdCopy(Cv::Image* img)
{
	return 0;
}

} // end namespace Img

