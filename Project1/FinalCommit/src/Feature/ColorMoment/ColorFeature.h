#pragma once

#include <vector>
#include <opencv/cv.h>

class ColorFeature
{
public:
	ColorFeature(){}
	static std::vector<int> getFeature(IplImage * src);
};
