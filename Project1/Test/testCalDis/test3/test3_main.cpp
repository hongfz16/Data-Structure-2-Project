#include "../../../Lib/RTree.h"
#include "../../../Lib/Rectangle.h"
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <ctime>
#include <cmath>
#include <algorithm>
using namespace std;

#define TESTNUM 50000
#define MULT 20
const int cdim = 24;

struct Datainfo
{
	int datanum;
	int classnum;
	int dim;
	vector<double> minbound;
	vector<double> maxbound;
	vector<int> classcount;
};

struct Pic
{
	int id;
	string filename;
	string classname;
	int classid;
	int dim;
	vector<double> dims;

	Pic(int _id, string _filename, string _classname,
		int _classid, int _dim, vector<double> _dims)
	{
		id = _id;
		filename = _filename;
		classname = _classname;
		classid = _classid;
		dim = _dim;
		dims = _dims;
	}
};

void printPics(vector<Pic>& pics)
{
	for (int i = 0; i < pics.size(); ++i)
	{
		cout << pics[i].id << " " << pics[i].dim << " " << pics[i].classname << endl;
	}
}

void printPic(Pic& pic)
{
	cout << pic.id << " " << pic.classid << " " << endl;
	for (int i = 0; i < pic.dim; ++i)
	{
		cout << pic.dims[i] << " ";
	}
	cout << endl;
}

struct Result
{
	double disktime;
	double accur;
	double recall;
	double resultnum;
	double points;
	Result(double _disktime, double _accur, double _recall, double _resultnum, double _points)
	{
		disktime = _disktime;
		accur = _accur;
		recall = _recall;
		resultnum = _resultnum;
		points = _points;
	}
};

RTree<Pic*, double, cdim> rt;

void initProvidedData(vector<Pic>& pics, Datainfo& datainfo, string datafilename, string datainfofilename)
{
	ifstream infofin(datainfofilename);
	ifstream datafin(datafilename);
	int idcount = 0;
	int classidcount = 0;
	int dim = 9;
	string line, filename, classname;
	vector<double> dims;
	set<string> classnames;
	getline(datafin, line);
	getline(datafin, line);
	datainfo.minbound.resize(dim, INT_MAX);
	datainfo.maxbound.resize(dim, 0);
	while (getline(infofin, filename))
	{
		getline(datafin, line);
		classname.clear();
		dims.clear();
		int linecount = 0;
		for (; line[linecount] != ' '; ++linecount);
		for (int i = 0; i < dim; ++i)
		{
			int tempdim = 0;
			for (++linecount; linecount < line.size(); ++linecount)
			{
				if (line[linecount] != ' ')
				{
					tempdim *= 10;
					tempdim += line[linecount] - '0';
				}
				else
				{
					break;
				}
			}
			dims.push_back(static_cast<double>(tempdim) / MULT);
			if (static_cast<double>(tempdim) / MULT > datainfo.maxbound[i])
				datainfo.maxbound[i] = static_cast<double>(tempdim) / MULT;
			if (static_cast<double>(tempdim) / MULT < datainfo.minbound[i])
				datainfo.minbound[i] = static_cast<double>(tempdim) / MULT;
		}
		for (int i = 0; filename[i] != '_'; ++i)
		{
			classname += filename[i];
		}
		++idcount;
		if (classnames.find(classname) == classnames.end())
		{
			classnames.insert(classname);
			++classidcount;
			datainfo.classcount.push_back(1);
		}
		else
		{
			datainfo.classcount[classidcount - 1]++;
		}

		Pic pic(idcount, filename, classname, classidcount, dim, dims);
		pics.push_back(pic);
	}
	datainfo.classnum = classidcount;
	datainfo.datanum = idcount;
	datainfo.dim = dim;
}

void initdata(vector<Pic>& pics, Datainfo& datainfo, string datafilename)
{
	int idcount = 0;
	int classidcount = 0;
	int dim = 0;
	ifstream fin(datafilename);
	string line, filename, classname;
	vector<double> dims;
	set<string> classnames;
	getline(fin, line);
	for (int i = 0; i < line.size(); ++i)
	{
		dim *= 10;
		dim += line[i] - '0';
	}
	datainfo.minbound.resize(dim, INT_MAX);
	datainfo.maxbound.resize(dim, 0);
	while (getline(fin, line))
	{
		filename.clear();
		classname.clear();
		dims.clear();
		int linecount = 0;
		for (; linecount < line.size(); ++linecount)
		{
			if (line[linecount] != ' ')
				filename += line[linecount];
			else
				break;
		}
		for (++linecount; linecount < line.size(); ++linecount)
		{
			if (line[linecount] != ' ')
				classname += line[linecount];
			else
				break;
		}
		for (int i = 0; i < dim; ++i)
		{
			int tempdim = 0;
			for (++linecount; linecount < line.size(); ++linecount)
			{
				if (line[linecount] != ' ')
				{
					tempdim *= 10;
					tempdim += line[linecount] - '0';
				}
				else
				{
					break;
				}
			}
			dims.push_back(static_cast<double>(tempdim) / MULT);
			if (static_cast<double>(tempdim) / MULT > datainfo.maxbound[i])
				datainfo.maxbound[i] = static_cast<double>(tempdim) / MULT;
			if (static_cast<double>(tempdim) / MULT < datainfo.minbound[i])
				datainfo.minbound[i] = static_cast<double>(tempdim) / MULT;
		}
		++idcount;
		if (classnames.find(classname) == classnames.end())
		{
			classnames.insert(classname);
			++classidcount;
			datainfo.classcount.push_back(1);
		}
		else
		{
			datainfo.classcount[classidcount - 1]++;
		}

		Pic pic(idcount, filename, classname, classidcount, dim, dims);
		//printPic(pic);
		pics.push_back(pic);
	}
	datainfo.classnum = classidcount;
	datainfo.datanum = idcount;
	datainfo.dim = dim;
}


bool _cdecl QueryResultCallback(Pic* a_data, vector<int>& resultid)
{
	resultid.push_back(a_data->id);
	return true;
}

typedef double (*calcDistFunc)(vector<Pic>& pics, vector<int> diminuse, int id1, int id2);

double calcDist2(vector<Pic>& pics, vector<int> diminuse, int id1, int id2)
{
	double ret = 0.0;
	double co = 1000;
	for (int i = 0; i < diminuse.size(); ++i)
	{
		ret += pow(pics[id1 - 1].dims[diminuse[i]] - pics[id2 - 1].dims[diminuse[i]], 2) / co;
	}
	//cout << sqrt(ret) << endl;
	return sqrt(ret);
}

double calcDist1(vector<Pic>& pics, vector<int> diminuse, int id1, int id2)
{
	double ret = 0.0;
	double co = 1000;
	for (int i = 0; i < diminuse.size(); ++i)
	{
		ret += fabs(pics[id1 - 1].dims[diminuse[i]] - pics[id2 - 1].dims[diminuse[i]]) / co;
	}
	//cout << sqrt(ret) << endl;
	return sqrt(ret);
}

double calcDist3(vector<Pic>& pics, vector<int> diminuse, int id1, int id2)
{
	double ret = 0.0;
	double co = 1000;
	for (int i = 0; i < diminuse.size(); ++i)
	{
		ret += pow(pics[id1 - 1].dims[diminuse[i]] - pics[id2 - 1].dims[diminuse[i]], 3) / co;
	}
	//cout << sqrt(ret) << endl;
	return sqrt(ret);
}

bool cmp(pair<int, double>& v1, pair<int, double>& v2)
{
	return v1.second < v2.second;
}

Result testRtree(int objnum, double range, vector<Pic>& pics, Datainfo& datainfo, vector<int> diminuse, 
	calcDistFunc calcDist)
{
	rt.RemoveAll();
	srand((unsigned int)time(nullptr));
	set<int> chosenpics;
	for (int i = 0; i < objnum; ++i)
	{
		int id = rand() % datainfo.datanum;
		while (chosenpics.find(id) != chosenpics.end())
		{
			id = rand() % datainfo.datanum;
		}
		double cmin[cdim], cmax[cdim];
		for (int j = 0; j < diminuse.size(); ++j)
		{
			cmin[j] = cmax[j] = pics[id].dims[diminuse[j]];
		}
		rt.Insert(cmin, cmax, &pics[id]);
	}
	double alldisktime = 0.0, allaccur = 0.0, allrecall = 0.0;
	double resultnum = 0.0, allpoints = 0.0;
	for (int i = 0; i < TESTNUM; ++i)
	{
		vector<int> resultid;
		int id = rand() % datainfo.datanum;
		double cmin[cdim], cmax[cdim];
		for (int j = 0; j < diminuse.size(); ++j)
		{
			int dimid = diminuse[j];
			cmin[j] = pics[id].dims[dimid] - range;
			cmax[j] = pics[id].dims[dimid] + range;
			if (pics[id].dims[dimid] - range < datainfo.minbound[j])
				cmin[j] = datainfo.minbound[j];
			if (pics[id].dims[dimid] + range > datainfo.maxbound[j])
				cmax[j] = datainfo.maxbound[j];
		}
		rt.Search(cmin, cmax, QueryResultCallback, resultid);

		alldisktime += rt.disktime;
		int allresult = resultid.size();
		int hit = 0;
		resultnum += allresult;
		vector<pair<int, double> > rank;
		for (int k = 0; k < resultid.size(); ++k)
		{
			if (pics[resultid[k] - 1].classid == pics[id].classid)
			{
				hit++;
			}
			rank.push_back(pair<int, double>(resultid[k], calcDist(pics, diminuse, id + 1, resultid[k])));
		}
		sort(rank.begin(), rank.end(), cmp);
		double point = 0.0;
		double co = 100;
		for (int k = 0; k < rank.size() && k < 100; ++k)
		{
			if (pics[rank[k].first - 1].classid == pics[id].classid)
			{
				//point += co / rank[k].second;
				point += 100 - k;
			}
			else
			{
				//point -= co / rank[k].second;
				point -= 100 - k;
			}
		}
		if (allresult == 0)
			continue;
		allaccur += (static_cast<double>(hit) / allresult);
		allrecall += (static_cast<double>(hit) / datainfo.classcount[pics[id].classid - 1]);
		allpoints += point;
		//cout << point << endl;
	}
	Result result(alldisktime / TESTNUM, allaccur / TESTNUM, allrecall / TESTNUM, resultnum / TESTNUM, allpoints / TESTNUM);
	return result;
}

void printcsv(vector<double>& vec)
{
	for (int i = 0; i < vec.size() - 1; ++i)
	{
		cout << vec[i] << ",";
	}
	cout << vec[vec.size() - 1] << endl;
}

int main()
{
	const string data1filename = "../../../Feature/ColorMoment/feature.txt";
	const string prodatafilename = "../../../Feature/ProvidedFeature/color_feature.txt";
	const string imagelistfilename = "../../../Feature/ProvidedFeature/imagelist.txt";
	const string data2filename = "../../../Feature/ColorHistogram/colorhist.txt";
	vector<Pic> pics_colormoment;
	vector<Pic> pics_colorhisto;
	Datainfo datainfo1;
	Datainfo datainfo2;
	//initdata(pics_colormoment, datainfo1, data1filename);
	//initProvidedData(pics_colormoment, datainfo1, prodatafilename, imagelistfilename);
	initdata(pics_colorhisto, datainfo2, data2filename);
	int objnum = datainfo2.datanum;
	double range = 700;
	//cout << "Query Range: " << range << endl;
	//Result result1 = testRtree(objnum, range, pics_colormoment, datainfo1);
	//cout << "Color Moment Features result:" << endl;
	//cout << "Accuracy: " << result1.accur << endl;
	//cout << "Call Back: " << result1.recall << endl;
	//cout << "Result Number: " << result1.resultnum << endl;
	vector<int> diminuse;
	for (int i = 0; i < cdim; ++i)
	{
		diminuse.push_back(i);
	}
	//cout << "Color Histogram Features result:" << endl;
	cout << "Color Histogram (24D) Features result:" << endl;
	Result result2 = testRtree(objnum, range, pics_colorhisto, datainfo2, diminuse,calcDist1);
	cout << "Distance 1:" << endl;
	cout << "Accuracy: " << result2.accur << endl;
	cout << "Call Back: " << result2.recall << endl;
	cout << "Result Number: " << result2.resultnum << endl;
	cout << "Relevance point: " << result2.points << endl;
	result2 = testRtree(objnum, range, pics_colormoment, datainfo1, diminuse, calcDist2);
	cout << "Distance 2:" << endl;
	cout << "Accuracy: " << result2.accur << endl;
	cout << "Call Back: " << result2.recall << endl;
	cout << "Result Number: " << result2.resultnum << endl;
	cout << "Relevance point: " << result2.points << endl;
	result2 = testRtree(objnum, range, pics_colormoment, datainfo1, diminuse, calcDist3);
	cout << "Distance 3:" << endl;
	cout << "Accuracy: " << result2.accur << endl;
	cout << "Call Back: " << result2.recall << endl;
	cout << "Result Number: " << result2.resultnum << endl;
	cout << "Relevance point: " << result2.points << endl;
	return 0;
}