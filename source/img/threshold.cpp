#include "threshold.hpp"
#include "grayscale.hpp"
#include "../cv/Image.hpp"
#include <cv.h>
#include <stdlib.h>

#include <vector>

using namespace std;

namespace Img {


/**
 * Threshold the image into the number of regions supplied.
 */
void threshold(Cv::Image* img, int numRegions)
{
	const int ITER_MAX = 10;
	unsigned int width, height, area;
	unsigned int i, j, k;

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
	for(i=0; i < threshPts.size(); i++) {
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

	while(!stable && iterCnt < ITER_MAX) 
	{
		// Sum the groups
		sums = vector<long>(numRegions, 0);
		cnts = vector<int>(numRegions, 0);
		
		for(i = 0; i < height; i++) {
			for(j = 0; j < width; j++) {
				int p = pix[i][j].r;
				bool assigned = false;

				// pixel must fall between threshold points to be assigned
				for(k = 0; k < threshPts.size(); k++) {
					if(k == 0) {
						if (p <= threshPts[0])
							assigned = true;
						else
							continue;
					}
					else {
						if(p <= threshPts[k]) // && p > threshPts[k-1])
							assigned = true;
					}

					if(assigned) {
						sums[k] += p;
						cnts[k]++;
						break;
					}
				}
				if(!assigned) {
					sums[sums.size()-1] += p;
					cnts[sums.size()-1]++;
				}
			}
		}
		
		stable = true;

		// Calculate mean of each grouping and determine stability
		for(i = 0; i < avgs.size(); i++) {
			avgsOld[i] = avgs[i];
			avgs[i] = 0;
			if(cnts[i] > 0) {
				avgs[i] = sums[i] / cnts[i];
			}
			if(avgsOld[i] != avgs[i]) {
				stable = false;
			}
		}

		// Create new threshold points as the average between groupings
		for(i = 0; i < threshPts.size(); i++) {
			threshPts[i] = (avgs[i] + avgs[i+1]) / 2;
		}

		iterCnt++;
	}

	// Apply thresholds
	for(i = 0; i < height; i++) {
		for(j = 0; j < width; j++) {
			bool set = false;
			for(k = 0; k < threshPts.size(); k++) {
				if(pix[i][j].r <= threshPts[k]) {
					pix[i][j].r = (step*k) % 255;
					pix[i][j].g = (step*k) % 255;
					pix[i][j].b = (step*k) % 255;
					set = true;
					break;
				}
			}
			if (!set) {
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

} // end namespace Img

