#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>
#include <algorithm>

#include "ColorFeature.h"
#include <omp.h>

using namespace cv;
using namespace std;

std::vector<int> ColorFeature::getFeature(IplImage * src)
{
	CvScalar s;
	vector<int> feature;
	feature.resize(9);
	vector<double> ans;
	ans.resize(9);

	//mean
#pragma omp parallel for
	for (int k = 0; k < 3; k++)
	{
		vector<double> sum;
		sum.resize(src->height);
		ans[k] = 0;
#pragma omp parallel for
		for (int i = 0; i < src->height; i++)
		{
			sum[i] = 0;
			for (int j = 0; j < src->width; j++)
			{
				s = cvGet2D(src, i, j);
				sum[i] += s.val[2 - k];
			}
			sum[i] = sum[i] / src->width;
		}
		for (int i = 0; i < src->height; i++)
		{
			ans[k] += sum[i];
		}
		ans[k] = ans[k] / src->height;
	}

	//standard variance
#pragma omp parallel for
	for (int k = 0; k < 3; k++)
	{
		vector<double> sum;
		sum.resize(src->height);
		ans[k + 3] = 0;
#pragma omp parallel for
		for (int i = 0; i < src->height; i++)
		{
			sum[i] = 0;
			for (int j = 0; j < src->width; j++)
			{
				s = cvGet2D(src, i, j);
				sum[i] += (s.val[2 - k] - ans[k]) * (s.val[2 - k] - ans[k]);
			}
			sum[i] = sum[i] / src->width;
		}
		for (int i = 0; i < src->height; i++)
		{
			ans[k + 3] += sum[i];
		}
		ans[k + 3] = ans[k + 3] / src->height;
		ans[k + 3] = sqrt(ans[k + 3]);
	}

	//skewness
#pragma omp parallel for
	for (int k = 0; k < 3; k++)
	{
		vector<double> sum;
		sum.resize(src->height);
		ans[k + 6] = 0;
#pragma omp parallel for
		for (int i = 0; i < src->height; i++)
		{
			sum[i] = 0;
			for (int j = 0; j < src->width; j++)
			{
				s = cvGet2D(src, i, j);
				double tmp = fabs(s.val[2 - k] - ans[k]);
				sum[i] += tmp * tmp * tmp;
			}
			sum[i] = sum[i] / src->width;
		}
		for (int i = 0; i < src->height; i++)
		{
			ans[k + 6] += sum[i];
		}
		ans[k + 6] = ans[k + 6] / src->height;
		ans[k + 6] = pow(ans[k + 6], 1.0 / 3);
	}

	//discretization
#pragma omp parallel for
	for (int i = 0; i < 9; i++)
	{
		feature[i] = (int)ans[i];
	}
	return feature;
}