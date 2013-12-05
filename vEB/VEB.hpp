#ifndef _VEB_HPP_
#define _VEB_HPP_


template <class T, unsigned h>
class VEB
{
	template <class U, unsigned a>
	friend class VEB;

private:
	VEB<T, h-h/2> top;
	VEB<T, h/2> down[(1<<(h-h/2))];

	static unsigned long long low(unsigned long long u)
	{
		return u & ((1ull<<(h/2))-1);
	}
	
	static unsigned long long high(unsigned long long u)
	{
		return u >> (h/2);
	}

	/*
	 * @return -1 wenn k gefunden, sonst den Index des Subtrees, in dem weitergesucht werden muss.
	 */
	long long psearch(const T& k) const
	{
		int child = top.psearch(k);
		if(child < 0)
			return -1;
		int child2 = down[child].psearch(k);
		if(child2 < 0)
			return -1;
		return (1<<(h/2))*child + child2;
	}

public:

	T& operator [] (unsigned long long index)
	{
		if(low(index) == low((unsigned long long)-1))
			return top[high(index)];
		return down[high(index)][low(index)];
	}

	bool search(const T& k) const
	{
		return psearch(k) < 0;
	}

};


template <class T>
class VEB<T, 1>
{
	template <class U, unsigned a>
	friend class VEB;

private:
	T data;

	int psearch(const T& k) const
	{
		if(k < data)
			return 0;
		if(k > data)
			return 1;
		return -1;
	}

public:
	T& operator [] (unsigned index)
	{
		return data;
	}

	bool search(const T& k) const
	{
		return psearch(k) < 0;
	}
};



#endif
