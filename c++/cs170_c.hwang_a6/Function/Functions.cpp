#include <iostream> // cout, endl

/******************************************************************************/
/*!
    Swaps two objects. There is no return value but the two objects are
    swapped in place.
  
  \param left
    The first object to swap.
  
  \param right
    The second object to swap.
*/
/******************************************************************************/
namespace CS170
{
	template <typename T>
	void swap(T& left, T& right)
	{
		T temp(right); // need a temporary copy
		right = left;
		left = temp;
	}

	template <typename T1, typename T2>
	int count(T1 begin, T1 end, const T2& value)
	{
		int count = 0;
		while (begin != end)
		{
			if (*begin == value)
			{
				count++;
			}
			begin++;
		}
		return count;
	}

	template <typename T1, typename T2>
	T2 copy(T1 begin1, T1 end1, T2 begin2)
	{
		while (begin1 != end1)
		{
			*begin2 = *begin1;
			begin1++;
			begin2++;
		}
		return begin2;
	}

	template <typename T>
	void display(T begin, T end)
	{
		std::cout << *begin;
		begin++;
		while (begin != end)
		{
			std::cout << ", " << *begin;
			begin++;
		}
		std::cout << std::endl;
	}
	template <typename T1, typename T2>
	bool equal(T1 begin1, T1 end2, T2 begin)
	{
		while (begin1 != end2)
		{
			if (*begin1 != *begin)
			{
				return false;
			}
			begin1++;
			begin++;
		}
		return true;
	}
	template <typename T1, typename T2>
	void fill(T1 begin, T1 end, const T2& value)
	{
		while (begin != end)
		{
			*begin = value;
			begin++;
		}
	}
	template <typename T1, typename T2>
	T1 find(T1 begin, T1 end, const T2& value)
	{
		while (begin != end)
		{
			if (*begin == value)
			{
				return begin;
			}
			begin++;
		}
		return end;
	}
	template <typename T>
	T max_element(T begin, T end)
	{
		T maxvalue = begin;
		begin++;
		while (begin != end)
		{
			if (*maxvalue < *begin)
			{
				maxvalue = begin;
			}
			begin++;
		}
		return maxvalue;
	}
	template <typename T>
	T min_element(T begin, T end)
	{
		T minvalue = begin;
		begin++;
		while (begin != end)
		{
			if (*minvalue > *begin)
			{
				minvalue = begin;
			}
			begin++;
		}
		return minvalue;
	}
	template <typename T1, typename T2>
	void replace(T1 begin, T1 end, const T2& oldItem, const T2& newItem)
	{
		while (begin != end)
		{
			if (*begin == oldItem)
			{
				*begin = newItem;
			}
			begin++;
		}
	}
	template <typename T>
	int sum(T begin, T end)
	{
		int sum = 0;
		while (begin != end)
		{
			sum += *begin;
			begin++;
		}
		return sum;
	}

	template <typename T1, typename T2>
	void swap_ranges(T1 begin1, T1 end1, T2 begin2)
	{
		while (begin1 != end1)
		{
			swap(*begin1, *begin2);
			begin1++;
			begin2++;
		}
	}
}