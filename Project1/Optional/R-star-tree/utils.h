#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <cmath>
#include <cmath>
#include <algorithm>
#include "RStarTree.h"
using namespace std;
using std::cout;
using std::endl;

#define TESTNUM 50000
#define MULT 1
const int cdim = 9;


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

typedef RStarTree<Pic*, cdim, 4, 8> RTree;

RTree rt;

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


struct Visitor {
	int count;
	bool ContinueVisiting;
	vector<Pic*> resultId;

	Visitor() : count(0), ContinueVisiting(true) {};

	void operator()(const RTree::Leaf * const leaf)
	{
#if defined( RANDOM_DATASET )
		resultId.push_back(leaf->leaf);
		//std::cout << "Visiting " << count << std::endl;
#elif defined( GUTTMAN_DATASET )
		std::cout << "#" << count << ": visited " << leaf->leaf << " with bound " << leaf->bound.ToString() << std::endl;
#else
		//#error "Undefined dataset"
#endif
		resultId.push_back(leaf->leaf);
		count++;
	}
};

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

Result testRtree(int objnum, double range, vector<Pic>& pics, Datainfo& datainfo, vector<int> diminuse)
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
		int cmin[cdim], cmax[cdim];
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
		int cmin[cdim], cmax[cdim];
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
		//rt.disktime = 0;
		Visitor x=rt.Search(cmin, cmax, RTree::AcceptAny(), Visitor());

		alldisktime += rt.disktime;
		//alldisktime += x.count;
		int allresult = x.resultId.size();
		int hit = 0;
		resultnum += allresult;
		vector<pair<int, double> > rank;
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
		allpoints += 0;
		//cout << point << endl;
	}
	Result result(alldisktime / TESTNUM, allaccur / TESTNUM, allrecall / TESTNUM, resultnum / TESTNUM, allpoints / TESTNUM);
	return result;
}