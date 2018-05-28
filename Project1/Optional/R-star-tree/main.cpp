/*
 *  Copyright (c) 2008 Dustin Spicuzza <dustin@virtualroadside.com>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of version 2.1 of the GNU Lesser General Public
 *  License as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 * 
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */


#include <string>
#include <ctime>
#include <vector>

#include <stdio.h>
#include "RStarTree.h"
#include "utils.h"

using namespace std;

//#define RANDOM_DATASET
//#define GUTTMAN_DATASET
#define COLORMOMENT

#ifdef RANDOM_DATASET
	typedef RStarTree<int, 2, 2, 4> 			RTree;
#endif
#ifdef GUTTMAN_DATASET
	typedef RStarTree<std::string, 2, 2, 3> 	RTree;
#endif
#ifdef COLORMOMENT
	
#endif

typedef RTree::BoundingBox			BoundingBox;


BoundingBox bounds(int x, int y, int w, int h)
{
	BoundingBox bb;
	
	bb.edges[0].first  = x;
	bb.edges[0].second = x + w;
	
	bb.edges[1].first  = y;
	bb.edges[1].second = y + h;
	
	return bb;
}



int main(int argc, char ** argv)
{
	//RTree tree;
	Visitor x;
	
	// insert a bunch of items into the tree
	// Note: this dataset is the one shown on Guttman's original paper
#ifdef GUTTMAN_DATASET
	tree.Insert( "R8" , bounds( 1,5   , 3,2 ));
	//tree.Print("I1");
	
	tree.Insert( "R9", bounds( 6,1   , 2,2 ));
	//tree.Print("I2");
	
	tree.Insert( "R10", bounds( 6,4   , 2,2 ));
	//tree.Print("I3");
	
	tree.Insert( "R11", bounds( 9,0   , 2,14 ));
	//tree.Print("I4");
	
	tree.Insert( "R13", bounds( 13,1  , 1,9 ));
	//tree.Print("I5");
	
	tree.Insert( "R14", bounds( 12,5  , 2,2 ));
	//tree.Print("I6");
	
	tree.Insert( "R15", bounds( 0,16  , 2,2 ));
	//tree.Print("I7");
	
	tree.Insert( "R16", bounds( 3,11  , 6,7 ));
	//tree.Print("I8");
	
	tree.Insert( "R17", bounds( 14,10 , 7,4 ));
	//tree.Print("I9");
	
	tree.Insert( "R18", bounds( 16,8  , 2,9 ));
	//tree.Print("I10");
	
	tree.Insert( "R19", bounds( 17,12 , 3,3 ));
	//tree.Print("I11");
	
	BoundingBox bound = bounds( 5,10, 5,5 );
	
	std::cout << "Searching in " << bound.ToString() << std::endl;
	x = tree.Query(RTree::AcceptOverlapping(bound), Visitor());
	std::cout << "Visited " << x.count << " nodes." << std::endl;
	
	tree.RemoveBoundedArea(bound);
	
	// stretch the bounds a bit
	
	std::cout << "Searching in " << bound.ToString() << std::endl;
	x = tree.Query(RTree::AcceptOverlapping(bound), Visitor());
	std::cout << "Visited " << x.count << " nodes." << std::endl;
	
	BoundingBox bound2 = bounds(0,10, 10,10);
	std::cout << "Removing enclosed area " << bound2.ToString() << std::endl;
	tree.RemoveBoundedArea(bound2);
	
	std::cout << "Searching in " << bound.ToString() << std::endl;
	x = tree.Query(RTree::AcceptOverlapping(bound), Visitor());
	std::cout << "Visited " << x.count << " nodes." << std::endl;
	
	
	Visitor y = tree.Query(RTree::AcceptAny(), Visitor());
	std::cout << "Visited " << y.count << " nodes." << std::endl;
	
	
#endif
	
#ifdef COLORMOMENT
	vector<Pic> colormoment;
	Datainfo datainfo;
	initdata(colormoment, datainfo, "../../Feature/ColorMoment/feature.txt");
	vector<int> diminuse;
	for (int i = 0; i < cdim; ++i)
	{
		diminuse.push_back(i);
	}
	Result result = testRtree(5000, 20, colormoment, datainfo, diminuse);
	std::cout << "Disk Time: " << result.disktime << std::endl;
	std::cout << "Retrived Number: " << result.resultnum << std::endl;
#endif

#ifdef RANDOM_DATASET
	srand(time(0));

	#define nodes 20
	
	int a_min[2], a_max[2];
	for (int i = 0; i < nodes; i++)
	{
		//tree.Insert(i, bounds( rand() % 1000, rand() % 1000, rand() % 10, rand() % 10));
		for (int j = 0; j < 2; ++j)
		{
			a_min[j] = a_max[j] = rand() % 1000;
		}
		tree.Insert(a_min, a_max, i);
	}
		
	//for (int i = 0; i < nodes/2; i++)
		//tree.Insert(i, bounds( rand() % 1000, rand() % 1000, rand() % 20, rand() % 20));
		
	BoundingBox bound = bounds( 100,100, 300,400 );
	int s_min[2] = { 100, 100 };
	int s_max[2] = { 400, 500 };
	std::cout << "Searching in " << bound.ToString() << std::endl;
	x = tree.Search(s_min, s_max, RTree::AcceptAny(), Visitor());
	std::cout << "disktime:" << tree.disktime << std::endl;
	std::cout << "Visited " << x.count << " nodes (" << tree.GetSize() << " nodes in tree)" << std::endl;
	std::cout << "result:" << std::endl;
	for (auto iter = x.resultId.begin(); iter != x.resultId.end(); ++iter)
	{
		std::cout << *iter << std::endl;
	}

	/*
	x = tree.Query(RTree::AcceptAny(), Visitor());
	std::cout << "disktime:" << tree.disktime << std::endl;
	std::cout << "AcceptAny: " << x.count << " nodes visited (" << tree.GetSize() << " nodes in tree)" << std::endl;
	
	
	std::cout << "Searching in " << bound.ToString() << std::endl;
	x = tree.Query(RTree::AcceptEnclosing(bound), Visitor());
	std::cout << "disktime:" << tree.disktime << std::endl;
	std::cout << "Visited " << x.count << " nodes (" << tree.GetSize() << " nodes in tree)" << std::endl;
	
	std::cout << "Removing enclosed area " << bound.ToString() << std::endl;
	tree.RemoveBoundedArea(bound);
	
	std::cout << "Searching in " << bound.ToString() << std::endl;
	x = tree.Query(RTree::AcceptEnclosing(bound), Visitor());
	std::cout << "disktime:" << tree.disktime << std::endl;
	std::cout << "Visited " << x.count << " nodes. (" << tree.GetSize() << " nodes in tree)" << std::endl;
	*/
	//tree.Print();
		
#endif

	
	
	return 0;
}


/*

http://donar.umiacs.umd.edu/quadtree/rectangles/cifquad.html

1.0 5.0  3.0 2.0
6.0 1.0  2.0 2.0
6.0 4.0 2.0 2.0
9.0 0.0  2.0 14.0
13.0 1.0  1.0 9.0
12.0 5.0  2.0 2.0
0.0 16.0  2.0 2.0
3.0 11.0  6.0 7.0
14.0 10.0  7.0 4.0
16.0 8.0  2.0 9.0
17.0 12.0  3.0 3.0



Insert-BoundingBox{(1.0,5.0)(4.0,7.0)}
Insert-BoundingBox{(6.0,1.0)(8.0,3.0)}
Insert-BoundingBox{(6.0,4.0)(8.0,6.0)}
Insert-BoundingBox{(9.0,0.0)(11.0,14.0)}
Insert-BoundingBox{(13.0,1.0)(14.0,1.0)}
Insert-BoundingBox{(12.0,5.0)(14.0,7.0)}
Insert-BoundingBox{(0.0,16.0)(2.0,18.0)}
Insert-BoundingBox{(3.0,11.0)(9.0,18.0)}
Insert-BoundingBox{(14.0,10.0)(21.0,14.0)}
Insert-BoundingBox{(16.0,8.0)(18.0,17.0)}
Insert-BoundingBox{(17.0,12.0)(20.0,15.0)}

*/


