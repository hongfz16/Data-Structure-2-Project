#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

void printtemp(ofstream& fout, vector<string>& v)
{
	fout<<"{";
	for(int i=0;i<v.size()-1;++i)
	{
		fout<<v[i]<<",";
	}
	fout<<v[v.size()-1]<<"}"<<endl;
}

int main()
{
	string filename("updateresult.txt");
	ifstream fin(filename);
	ofstream fout("temp.txt");
	string line;
	int globcount=0;
	vector<string> v1,v2,v3,v4;
	while(getline(fin,line))
	{
		if(line.empty())
			continue;
		string temp;
		int count=0;
		for(;count<line.length() && line[count]!=':';++count);
		count+=2;
		for(;count<line.length();++count)
		{
			temp+=line[count];
		}
		if(globcount%4==0)
			v1.push_back(temp);
		else if(globcount%4==1)
			v2.push_back(temp);
		else if(globcount%4==2)
			v3.push_back(temp);
		else
			v4.push_back(temp);
		globcount++;
	}
	printtemp(fout,v1);
	printtemp(fout,v2);
	printtemp(fout,v3);
	printtemp(fout,v4);
}