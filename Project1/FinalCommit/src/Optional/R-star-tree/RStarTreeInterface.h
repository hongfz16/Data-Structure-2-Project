#ifndef RSTInterface
#define RSTInterface

#include "RStarTree.h"
/*
template <
	typename LeafType,
	std::size_t dimensions, std::size_t min_child_items, std::size_t max_child_items
>
class RSTree :public RStarTree<LeafType, dimensions, min_child_items, max_child_items>
{
	RSTree() :RStarTree() {}
	~RSTree()
	{
		Remove(AcceptAny(), RemoveLeaf());
	}

public:
	void RemoveAll()
	{
		Remove(AcceptAny(), RemoveLeaf());
	}
	void Insert(int a_min[dimensions], int a_max[dimensions], const LeafType& a_dataId);
	
	template <typename Acceptor, typename Visitor>
	void Search(int a_min[dimensions], int a_max[dimensions], const Acceptor& accpet, Visitor visitor);
};
*/	
#endif // !RSTInterface