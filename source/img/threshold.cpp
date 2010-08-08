#include "threshold.hpp"
#include "grayscale.hpp"
#include "../cv/Image.hpp"
#include <cv.h>
#include <stdlib.h>

#include <vector>

using namespace std;

namespace Img {

// XXX/TODO
void threshold(Cv::Image* img, int numRegions)
{
	const int ITER_MAX = 10;
	int width, height, area;

	// Region accumulation, averaging, and history
	vector<long> sums(numRegions, 0);
	vector<int> cnts(numRegions, 0);
	vector<int> avgs(numRegions, 0);
	vector<int> avgsOld(numRegions, 0);

	// Threshold points
	vector<int> threshPts(numRegions-1, 0);

	// TODO: Assert numRegions >= 2

	// Initially set region threshold points to be equal width
	int step = 255/numRegions;
	for(int i=0; i < threshPts.size(); i++) {
		threshPts[i] = (i+1)*step;
	}


	// Must operate on grayscale image
	grayscale(img);

	width = img->getWidth();
	height = img->getHeight();

	area = width*height;

	RgbPix pix = img->getPix(); // TODO: Rename Cv::Pix

	bool stable = false;
	int iterCnt = 0;

	// TODO TEMP
	vector<Color> colors(10, Color());
	colors[0] = Color("#FFFFFF");
	colors[1] = Color("#FF0000");
	colors[2] = Color("#00FF00");
	colors[3] = Color("#0000FF");

	while(!stable && iterCnt < ITER_MAX) 
	{
		// Sum the groups
		sums = vector<long>(numRegions, 0);
		for(int i = 0; i < height; i++) {
			for(int j = 0; j < width; j++) {
				int p = pix[i][j].r;
				bool found = false;
				for(int k = 0; k < threshPts.size(); k++) {
					if(p <= threshPts[k]) {
						sums[k] += p;
						cnts[k]++;
						found = true;
						break;
					}
				}
				if(!found) {
					sums[sums.size()-1] += p;
					cnts[sums.size()-1]++;
				}
			}
		}

		// Determine stability and calculate mean of each grouping
		bool stable = true;
		for(int i = 0; i < sums.size(); i++) {
			if(avgsOld[i] != avgs[i]) {
				stable = false;
			}
			avgsOld[i] = avgs[i];
			avgs[i] = sums[i] / cnts[i];
		}

		// Create new threshold points as the average between groupings
		for(int i = 0; i < threshPts.size(); i++) {
			threshPts[i] = (avgs[i] + avgs[i+1]) / 2;
		}

		iterCnt++;
	}

	// Apply thresholds
	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			bool set = false;
			for(int k = 0; k < threshPts.size(); k++) {
				if(pix[i][j].r < threshPts[i]) {
					pix[i][j].r = 255; //colors[k].r;
					pix[i][j].g = 255; //colors[k].g;
					pix[i][j].b = 255; //colors[k].b;
					set = true;
					break;
				}
			}
			if (!set) {
				pix[i][j].r = 0;
				pix[i][j].g = 0;
				pix[i][j].b = 0;
			}	
		}
	}
}

void binaryThreshold(Cv::Image* img)
{
	const int ITER_MAX = 10;
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

	int cnt = 0;

	while(!stable && cnt < ITER_MAX) {
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
		printf("Iteration %d, threshold is: %d\n", cnt, thresh);
		cnt++;
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

