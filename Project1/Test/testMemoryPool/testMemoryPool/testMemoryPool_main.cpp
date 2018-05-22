#include "../../../Lib/RTreeMemPool.h"

#include "../../../Lib/Rectangle.h"
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <ctime>
using namespace std;

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
	double averageinserttime;
	double averagedeletetime;
	Result(double _av1, double _av2)
	{
		averageinserttime = _av1;
		averagedeletetime = _av2;
	}
};

vector<Pic> pics;
Datainfo datainfo;

const int cdim = 9;
RTree<Pic*, float, cdim> rt;

const string datafilename = "../../../Feature/ColorMoment/feature.txt";

void initdata()
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

Result testTime(int inserttime, int deletetime)
{
	rt.RemoveAll();
	srand((unsigned)time(nullptr));
	float cmin[cdim], cmax[cdim];
	clock_t start = clock();
	vector<int> inserted;
	inserted.resize(inserttime);
	for (int i = 0; i < inserttime; ++i)
	{
		int id =rand() % datainfo.datanum;
		for (int j = 0; j < cdim; ++j)
		{
			cmin[j] = cmax[j] = pics[id].dims[j]+rand()%1000;
		}
		rt.Insert(cmin, cmax, &pics[id]);
		inserted[i] = id;
	}
	clock_t time1 = clock() - start;
	for (int i = 0; i < inserttime; ++i)
	{
		int p1 = rand() % inserttime;
		int p2 = rand() % inserttime;
		int temp = inserted[p1];
		inserted[p1] = inserted[p2];
		inserted[p2] = temp;
	}
	start = clock();
	for (int i = 0; i < inserttime; ++i)
	{
		int id = inserted[i];
		for (int j = 0; j < cdim; ++j)
		{
			cmin[j] = cmax[j] = pics[id].dims[j]+rand()%1000;
		}
		rt.Remove(cmin, cmax, &pics[id]);
	}
	clock_t time2 = clock() - start;
	Result result(static_cast<double>(time1) / inserttime, static_cast<double>(time2) / deletetime);
	return result;
}

int main()
{
	rt.setMemPoolValid(true);
	rt.setSpherVolValid(false);

	initdata();

	cout << "Using memory pool" << endl;
	Result result = testTime(5000, 5000);
	cout << "Average Insert Time: " << result.averageinserttime << "ms." << endl;
	cout << "Average Delete Time: " << result.averagedeletetime << "ms." << endl;
	
	cout << "Not using memory pool" << endl;
	rt.RemoveAll();
	rt.setMemPoolValid(false);
	Result result2 = testTime(5000, 5000);
	cout << "Average Insert Time: " << result2.averageinserttime << "ms." << endl;
	cout << "Average Delete Time: " << result2.averagedeletetime << "ms." << endl;

	return 0;
}