#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <vector>

template <class ELEMTYPE, int NUMDIMS>
struct Rect
{
	Rect() {}
	Rect(ELEMTYPE a_min[NUMDIMS], ELEMTYPE a_max[NUMDIMS])
	{
		for (int i = 0; i < NUMDIMS; ++i)
		{
			min[i] = a_min[i];
			max[i] = a_max[i];
		}
	}

	Rect(const std::vector<ELEMTYPE>& a_min, const std::vector<ELEMTYPE>& a_max)
	{
		for (auto min_iter = a_min.begin(); min_iter != a_min.end(); ++min_iter)
		{
			min[i] = *min_iter;
		}
		for (auto max_iter = a_max.begin(); max_iter != a_max.begin(); ++max_iter)
		{
			max[i] = *max_iter;
		}
	}

	ELEMTYPE min[NUMDIMS];
	ELEMTYPE max[NUMDIMS];
};

#endif // !RECTANGLE_H

