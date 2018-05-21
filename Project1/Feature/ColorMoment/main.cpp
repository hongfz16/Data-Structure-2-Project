#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <opencv/cv.h>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\ml\ml.hpp>

#include "global.h"
#include "ColorFeature.h"


using namespace cv;
using namespace std;

#define TRAINING_NUM 4585
#define FEATURE_DIM 9

int main(int argc, char** argv)
{
	//extract features of training images
	ifstream trainingis;
	trainingis.open("test.txt", ios::in);
	ofstream trainingos;
	trainingos.open("testfeature.txt", ios::out | ios::app);
	string imagepre = "test/";
	string imagename;
	char label[20];
	while (!trainingis.eof())
	{
		trainingis >> imagename;
		IplImage * src = cvLoadImage((imagepre+imagename).c_str());
		getLabel(imagename, label);
		vector<int> feature = ColorFeature::getFeature(src);
		trainingos << label << " ";
		for (int i = 0; i < 9; i++)
		{
			trainingos << feature[i] << " ";
		}
		trainingos << endl;
		cout << imagename << endl;
		cvReleaseImage(&src);
	}
	trainingis.close();
	trainingos.close();

	return 0;
}