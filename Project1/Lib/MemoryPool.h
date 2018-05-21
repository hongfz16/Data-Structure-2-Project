#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include <deque>
using std::deque;

template <typename T>
class MemoryPool
{
public:
	MemoryPool(int _size):m_size(_size),m_curIndex(0)
	{
		m_pMemory = new T[_size];
		for(int i=0; i<_size; i++)
		{
			deque.push_back(i);
		}
	}
	~MemoryPool()
	{
		delete [] m_pMemory;
	}
	int rest()
	{
		return dq.size();
	}
	void reset()
	{
		deque.clear();
		for(int i=0; i<_size; i++)
		{
			deque.push_back(i);
		}
	}
	T* get()//get new memory
	{
		if(dq.empty())
		{
			return nullptr;
		}
		T* toUse = m_pMemory + dq.front();
		dq.pop_front();
		return toUse;
	}
	void retr(T* elem)
	{
		if(elem < m_pMemory || elem-m_pMemory >= size)
		{
			//a wrong retr operation out of range
		}
		else
		{
			dq.push_back(elem-m_pMemory);
		}
	}

private:
	int m_size;
	deque<int> dq;
	T* m_pMemory;
};


#endif MEMORYPOOL_H