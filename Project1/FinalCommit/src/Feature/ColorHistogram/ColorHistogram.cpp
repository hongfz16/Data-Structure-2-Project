#include <omp.h>
#include <iostream>
#include "ColorHistogram.h"

using namespace std;

vector<int> ColorHist::getColorHist(Mat& image, const int binsNum)
{
	vector<int> ColorHist(3 * binsNum, 0);
	int range = (1 << 8) / binsNum;
#pragma omp parallel for
	for (int i = 0; i < image.rows; ++i)
	{
#pragma omp parallel for
		for (int j = 0; j < image.cols; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				//cout << k * binsNum + image.at<Vec3b>(i, j)[k] / range << endl;
				ColorHist[k * binsNum + image.at<Vec3b>(i, j)[k] / range]++;
			}
		}
	}
	return ColorHist;
}