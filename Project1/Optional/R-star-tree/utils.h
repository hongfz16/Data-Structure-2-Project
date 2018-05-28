#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <cmath>
#include <cmath>
#include <algorithm>
using namespace std;

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