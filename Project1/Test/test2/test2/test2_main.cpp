#include "../../../Lib/RTree.h"
#include "../../../Lib/Rectangle.h"
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <ctime>
using namespace std;

#define TESTNUM 5000

struct Datainfo
{
	int datanum;
	int classnum;
	int dim;
	vector<int> minbound;
	vector<int> maxbound;
	vector<int> classcount;
};

struct Pic
{
	int id;
	string filename;
	string classname;
	int classid;
	int dim;
	vector<int> dims;

	Pic(int _id, string _filename, string _classname,
		int _classid, int _dim, vector<int> _dims)
	{
		id = _id;
		filename = _filename;
		classname = _classname;
		classid = _classid;
		dim = _dim;
		dims = _dims;
	}
};

struct Result
{
	double disktime;
	double accur;
	double recall;
	Result(double _disktime, double _accur, double _recall)
	{
		disktime = _disktime;
		accur = _accur;
		recall = _recall;
	}
};

const int cdim = 9;
RTree<Pic*, float, cdim> rt;

void initdata(vector<Pic>& pics, Datainfo& datainfo, string datafilename)
{
	int idcount = 0;
	int classidcount = 0;
	int dim = 0;
	ifstream fin(datafilename);
	string line, filename, classname;
	vector<int> dims;
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
			dims.push_back(tempdim);
			if (tempdim > datainfo.maxbound[i])
				datainfo.maxbound[i] = tempdim;
			if (tempdim < datainfo.minbound[i])
				datainfo.minbound[i] = tempdim;
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


bool _cdecl QueryResultCallback(Pic* a_data, vector<int>& resultid)
{
	resultid.push_back(a_data->id);
	return true;
}


Result testRtree(int objnum, int range, vector<Pic>& pics, Datainfo& datainfo)
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
		float cmin[cdim], cmax[cdim];
		for (int j = 0; j < cdim; ++j)
		{
			cmin[j] = cmax[j] = pics[id].dims[j];
		}
		rt.Insert(cmin, cmax, &pics[id]);
	}
	double alldisktime = 0.0, allaccur = 0.0, allrecall = 0.0;
	for (int i = 0; i < TESTNUM; ++i)
	{
		vector<int> resultid;
		int id = rand() % datainfo.datanum;
		float cmin[cdim], cmax[cdim];
		for (int j = 0; j < cdim; ++j)
		{
			cmin[j] = pics[id].dims[j] - range;
			cmax[j] = pics[id].dims[j] + range;
			if (pics[id].dims[j] - range < datainfo.minbound[j])
				cmin[j] = datainfo.minbound[j];
			if (pics[id].dims[j] + range > datainfo.maxbound[j])
				cmax[j] = datainfo.maxbound[j];
		}
		rt.Search(cmin, cmax, QueryResultCallback, resultid);
		alldisktime += rt.disktime;
		int allresult = resultid.size();
		int hit = 0;
		for (int k = 0; k < resultid.size(); ++k)
		{
			if (pics[resultid[k] - 1].classid == pics[id].classid)
			{
				hit++;
			}
		}
		if (allresult == 0)
			continue;
		allaccur += (static_cast<double>(hit) / allresult);
		allrecall += (static_cast<double>(hit) / datainfo.classcount[pics[id].classid - 1]);
	}
	Result result(alldisktime / TESTNUM, allaccur / TESTNUM, allrecall / TESTNUM);
	return result;
}

int main()
{
	const string data1filename = "../../../Feature/ColorMoment/feature.txt";
	const string data2filename = "../../../Feature/";
	vector<Pic> pics_colormoment;
	vector<Pic> pics_colorhisto;
	Datainfo datainfo1;
	Datainfo datainfo2;
	initdata(pics_colormoment, datainfo1, data1filename);
	initdata(pics_colorhisto, datainfo2, data2filename);
	int objnum = datainfo1.datanum;
	int range = 20;
	for (int i = 0; i < 5; ++i, range += 20)
	{
		Result result1 = testRtree(objnum, range, pics_colormoment, datainfo1);
		Result result2 = testRtree(objnum, range, pics_colormoment, datainfo2);
		cout << "Query Range: " << range << endl;
		cout << "Color Moment Features result:" << endl;
		cout << "Accuracy: " << result1.accur << endl;
		cout << "Call Back: " << result1.recall << endl;
		cout << "Color Histogram Features result:" << endl;
		cout << "Accuracy: " << result2.accur << endl;
		cout << "Call Back: " << result2.recall << endl;
	}
	return 0;
}