#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <opencv\cv.h>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>

#include "ColorHistogram.h"
#include "global.h"

using namespace std;
using namespace cv;

const int TRAIN_NUM = 4585;
const int BINS_NUM = 8;

int main(int argc, char *argv[])
{
	ifstream trainingis;
	trainingis.open("image_list.txt", ios::in);
	ofstream trainingos;
	trainingos.open("colorhist.txt", ios::out | ios::app);
	string pre = "image/";
	string imageName;
	char label[20];
	trainingos << "number of bins: " << BINS_NUM << endl;
	while (!trainingis.eof())
	{
		trainingis >> imageName;
		Mat src = imread((pre + imageName).c_str());
		if (!src.data)
			continue;
		getLabel(imageName, label);
		vector<int> colorHist = ColorHist::getColorHist(src, BINS_NUM);

		trainingos << imageName << " " << label << " ";
		for (int i = 0; i < BINS_NUM * 3; i++)
		{
			trainingos << colorHist[i] << " ";
		}
		trainingos << endl;
		cout << imageName << endl;
		src.release();
	}
	trainingis.close();
	trainingos.close();

	return 0;
}