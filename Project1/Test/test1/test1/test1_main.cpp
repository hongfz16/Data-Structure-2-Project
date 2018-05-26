#include "../../../Lib/RTree.h"
#include "../../../Lib/Rectangle.h"
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <ctime>
using namespace std;

#define TESTNUM 20000
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

struct Result
{
	double disktime;
	double accur;
	double recall;
	double resultnum;
	Result(double _disktime, double _accur, double _recall, double _resultnum)
	{
		disktime = _disktime;
		accur = _accur;
		recall = _recall;
		resultnum = _resultnum;
	}
};

vector<Pic> pics;
Datainfo datainfo;

RTree<Pic*, double, cdim> rt;

//const string datafilename = "../../../Feature/ColorMoment/feature.txt";
const string datafilename = "../../../Feature/ColorHistogram/colorhist.txt";
void initdata()
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
			double tempdim = 0;
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
			tempdim /= MULT;
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
			datainfo.classcount[classidcount-1]++;
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


Result testRtree(int objnum,int range)
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
		chosenpics.insert(id);
		double cmin[cdim], cmax[cdim];
		for (int j = 0; j < cdim; ++j)
		{
			cmin[j] = cmax[j] = pics[id].dims[j];
		}
		rt.Insert(cmin, cmax, &pics[id]);
	}
	double alldisktime=0.0, allaccur=0.0, allrecall=0.0, resultnum = 0.0;
	for (int i = 0; i < TESTNUM; ++i)
	{
		vector<int> resultid;
		int id = rand() % datainfo.datanum;
		while (1)
		{
			if (chosenpics.find(id) == chosenpics.end())
				id = rand() % datainfo.datanum;
			else
				break;
		}
		double cmin[cdim], cmax[cdim];
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
		resultnum += allresult;
		for (int k = 0; k < resultid.size(); ++k)
		{
			if (pics[resultid[k]-1].classid == pics[id].classid)
			{
				hit++;
			}
		}
		if (allresult == 0)
			continue;
		allaccur += (static_cast<double>(hit) / allresult);
		allrecall += (static_cast<double>(hit) / datainfo.classcount[pics[id].classid-1]);
	}
	Result result(alldisktime / TESTNUM, allaccur / TESTNUM, allrecall / TESTNUM, resultnum / TESTNUM);
	return result;
}

int main()
{
	initdata();
	int range = 500;
	int objnum[9] = { 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000 };
	for (int i = 0; i < 9; ++i)
	{
		Result result = testRtree(objnum[i], range);
		cout << "Dimension: " << cdim << endl;
		cout << "Object number: " << objnum[i] << endl;
		cout << "Average Disk Time: " << result.disktime << endl;
		cout << "Average Disk Time Per 100 Result: " << result.disktime / result.resultnum * 100 << endl;
	}
	return 0;
}