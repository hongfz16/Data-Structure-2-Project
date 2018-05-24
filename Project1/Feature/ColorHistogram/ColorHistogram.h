#ifndef COLORHIST
#define COLORHIST
#include <vector>
#include <opencv\cv.h>
using namespace cv;

class ColorHist
{
public:
	ColorHist() {}
	static std::vector<int> getColorHist(Mat& image, const int binsNum);
};

#endif // !COLORHIST
