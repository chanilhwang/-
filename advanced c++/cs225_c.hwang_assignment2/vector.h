#pragma once
#include <iostream>

namespace CS225
{
    struct ElementNode
    {
        int data;
        int pos;
		ElementNode* next;
    };
	class ElementProxy;

    class SparseVector
    {
    private:
		ElementNode* pHead;
        long dimension;

    public:
		SparseVector();
		SparseVector(const SparseVector& v);
		~SparseVector();

		int Get(int pos) const;
        void Insert(int val, int pos);
        void PrintRaw() const;
		void Delete(int pos);

		ElementProxy operator[](int pos);
		int operator[](int pos) const;
		SparseVector& operator=(const SparseVector& v);
		SparseVector operator+(const SparseVector& v) const;
        SparseVector operator*(int val) const;
		friend SparseVector operator*(int val, const SparseVector& myvec);
		int operator*(const SparseVector& v) const;

        friend std::ostream& operator<<(std::ostream& out,const SparseVector &v);
    };

	class ElementProxy
	{
	public:
		ElementProxy(SparseVector& v, int pos);

		ElementProxy& operator=(int val);
		ElementProxy& operator=(ElementProxy node);
		ElementProxy& operator+=(int val);
		ElementProxy& operator+=(ElementProxy node);
		ElementProxy& operator-=(int val);
		ElementProxy& operator-=(ElementProxy node);
		operator int () const;

	private:
		SparseVector& v;
		int pos;
	};
}