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

vector<Pic> pics;
Datainfo datainfo;

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
	cout << dim << endl;
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
					tempdim += line[linecount - '0'];
				}
				else
				{
					break;
				}
			}
			dims.push_back(tempdim);
		}

		struct Pic pic(idcount, filename, classname, classidcount, dim, dims);
		pics.push_back(pic);
		++idcount;
		if (classnames.find(classname) == classnames.end())
		{
			classnames.insert(classname);
			++classidcount;
			datainfo.classcount.push_back(1);
		}
		else
		{
			datainfo.classcount[classidcount]++;
		}
	}
	datainfo.classnum = classidcount + 1;
	datainfo.datanum = idcount + 1;
	datainfo.dim = dim;
}


bool _cdecl QueryResultCallback(Pic* a_data, vector<int>& resultid)
{
	resultid.push_back(a_data->id);
	return true;
}

const int cdim = 2;
RTree<Pic*, float, cdim> rt;

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
		float cmin[cdim], cmax[cdim];
		for (int j = 0; j < cdim; ++j)
		{
			cmin[j] = cmax[j] = pics[id].dims[j];
		}
		rt.Insert(cmin, cmax, &pics[id]);
	}
	double alldisktime, allaccur, allrecall;
	vector<int> resultid;
	for (int i = 0; i < TESTNUM; ++i)
	{
		resultid.clear();
		int id = rand() % datainfo.datanum;
		float cmin[cdim], cmax[cdim];
		for (int j = 0; j < cdim; ++j)
		{
			cmin[j] = pics[id].dims[j] - range;
			cmax[j] = pics[id].dims[j] + range;
		}
		rt.Search(cmin, cmax, QueryResultCallback, resultid);
		alldisktime += rt.disktime;
		int allresult = resultid.size();
		int hit = 0;
		for (int k = 0; k < resultid.size(); ++k)
		{
			if (pics[resultid[k]].classid == pics[id].classid)
			{
				hit++;
			}
		}
		allaccur += (static_cast<double>(hit) / allresult);
		allrecall += (static_cast<double>(hit) / datainfo.classcount[pics[id].classid]);
	}
	Result result(alldisktime / TESTNUM, allaccur / TESTNUM, allrecall / TESTNUM);
	return result;
}

int main()
{
	initdata();
	int range = 50;
	int objnum[5] = { 1000,2000,3000,4000,5000 };
	for (int i = 0; i < 5; ++i)
	{
		Result result = testRtree(objnum[i], range);
		cout << "Dimension: " << cdim << endl;
		cout << "Object number: " << objnum[i] << endl;
		cout << "Average Disk Time: " << result.disktime << endl;
		cout << "Average Accuracy: " << result.accur << endl;
		cout << "Average Recall: " << result.recall << endl;
	}
	return 0;
}