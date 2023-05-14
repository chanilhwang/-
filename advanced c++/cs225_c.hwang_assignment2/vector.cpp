#include "vector.h"

namespace CS225
{
	//SparseVector class area
	SparseVector::SparseVector() : pHead(nullptr), dimension(0)
	{}

	SparseVector::SparseVector(const SparseVector& v) : pHead(nullptr), dimension(0)
	{
		ElementNode* temp = v.pHead;
		while (temp)
		{
			Insert(temp->data, temp->pos);
			temp = temp->next;
		}
	}

	SparseVector::~SparseVector()
	{
		ElementNode* temp = pHead;
		ElementNode* temp2 = pHead;
		while (temp2)
		{
			temp = temp2;
			temp2 = temp->next;
			Delete(temp->pos);
		}
		return;
	}

	int SparseVector::Get(int pos) const
	{
		ElementNode* temp = pHead;
		while (temp)
		{
			if (temp->pos == pos)
			{
				return temp->data;
			}
			temp = temp->next;
		}
		return 0;
	}

	void SparseVector::Insert(int val, int pos)
	{
		if(val == 0)
		{
			Delete(pos);
			return;
		}
		if (pos > dimension)
		{
			dimension = pos + 1;
		}
		if(pHead == nullptr)
		{
			pHead = new ElementNode;
			pHead->data = val;
			pHead->pos = pos;
			pHead->next = nullptr;
		}
		else
		{
			ElementNode* temp = pHead;

			if (temp->pos == pos)
			{
				temp->data = val;
			}
			else if(temp->pos > pos)
			{
				temp = new ElementNode;
				temp->data = val;
				temp->pos = pos;
				temp->next = pHead;
				pHead = temp;
			}
			else
			{
				while (1)
				{
					if (temp->next == nullptr)
					{
						temp->next = new ElementNode;
						temp->next->data = val;
						temp->next->pos = pos;
						temp->next->next = nullptr;
						break;
					}
					else if (temp->next->pos == pos)
					{
						temp->next->data = val;
						break;
					}
					else if (temp->next->pos > pos)
					{
						ElementNode* temp2 = temp->next;
						temp->next = new ElementNode;
						temp->next->data = val;
						temp->next->pos = pos;
						temp->next->next = temp2;
						break;
					}
					else
					{
						temp = temp->next;
					}
				}
			}
		}
		return;
	}

	void SparseVector::PrintRaw() const
	{
		ElementNode* temp = pHead;
		std::cout << "Raw vector: ";
		while(temp)
		{
			std::cout << "(" << temp->data << ", " << temp->pos << ")";
			temp = temp->next;
		}
		std::cout << std::endl;
	}

	void SparseVector::Delete(int pos)
	{
		ElementNode* temp = pHead;
		if (temp == nullptr)
		{
			return;
		}
		else if (temp->pos == pos)
		{
			pHead = temp->next;
			delete(temp);
			temp = nullptr;
			return;
		}
		while (temp->next)
		{
			if (temp->next->pos == pos)
			{
				ElementNode* temp2 = temp->next;
				temp->next = temp->next->next;
				delete(temp2);
				temp2 = nullptr;
			}
			temp = temp->next;
		}
		return;
	}

	ElementProxy SparseVector::operator[](int pos)
	{
		ElementProxy p(*this, pos);
		return p;
	}

	int SparseVector::operator[](int pos) const
	{
		return Get(pos);
	}

	SparseVector& SparseVector::operator=(const SparseVector& v)
	{
		ElementNode* temp = v.pHead;
		while (temp)
		{
			this->Insert(temp->data, temp->pos);
			temp = temp->next;
		}
		return *this;
	}

	SparseVector SparseVector::operator+(const SparseVector& v) const
	{
		SparseVector vec;
		ElementNode* temp = pHead;
		ElementNode* temp2 = v.pHead;
		while (temp && temp2)
		{
			if (temp->pos > temp2->pos)
			{
				vec.Insert(temp2->data, temp2->pos);
				temp2 = temp2->next;
			}
			else if (temp->pos == temp2->pos)
			{
				vec.Insert(temp->data + temp2->data, temp->pos);
				temp = temp->next;
				temp2 = temp2->next;
			}
			else
			{
				vec.Insert(temp->data, temp->pos);
				temp = temp->next;
			}
		}
		if (temp)
		{
			while (temp)
			{
				vec.Insert(temp->data, temp->pos);
				temp = temp->next;
			}
		}
		else if (temp2)
		{
			while (temp2)
			{
				vec.Insert(temp2->data, temp2->pos);
				temp2 = temp2->next;
			}
		}
		return vec;
	}

	SparseVector SparseVector::operator*(int val) const
	{
		if(val == 0)
		{
			SparseVector vec;
			return vec;
		}
		else
		{
			SparseVector vec;
			ElementNode* temp = pHead;
			while(temp)
			{
				vec.Insert(temp->data * val, temp->pos);
				temp = temp->next;
			}
			return vec;
		}
	}

	SparseVector operator*(int val, const SparseVector& myvec)
	{
		return myvec * val;
	}

	int SparseVector::operator*(const SparseVector& v) const
	{
		int val = 0;
		ElementNode* temp = pHead;
		ElementNode* temp2 = v.pHead;
		while (temp && temp2)
		{
			if (temp->pos > temp2->pos)
			{
				temp2 = temp2->next;
			}
			else if (temp->pos == temp2->pos)
			{
				val += temp->data * temp2->data;
				temp = temp->next;
				temp2 = temp2->next;
			}
			else
			{
				temp = temp->next;
			}
		}
		return val;
	}

	//NodeProxy class area
	ElementProxy::ElementProxy(SparseVector& vec, int pos) : v(vec), pos(pos)
	{}

	ElementProxy& ElementProxy::operator=(int val)
	{
		v.Insert(val, pos);
		return *this;
	}

	ElementProxy& ElementProxy::operator=(ElementProxy node)
	{
		v.Insert(node.v.Get(node.pos), pos);
		return *this;
	}

	ElementProxy& ElementProxy::operator+=(int val)
	{
		v.Insert(v.Get(pos) + val, pos);
		return *this;
	}

	ElementProxy& ElementProxy::operator+=(ElementProxy node)
	{
		v.Insert(v.Get(pos) + node.v.Get(node.pos), pos);
		return *this;
	}

	ElementProxy& ElementProxy::operator-=(int val)
	{
		v.Insert(v.Get(pos) - val, pos);
		return *this;
	}

	ElementProxy& ElementProxy::operator-=(ElementProxy node)
	{
		v.Insert(v.Get(pos) - node.v.Get(node.pos), pos);
		return *this;
	}

	ElementProxy::operator int () const
	{
		return v.Get(pos);
	}

	/* this is the only place where dimension is used */
	std::ostream& operator<<(std::ostream &out, const SparseVector &v) {
		int i,last_pos=-1; 
		ElementNode* p_e=v.pHead; 
		while (p_e) { 
			for (i=last_pos+1;i<p_e->pos;++i) out << " " << "0"; 
			out << " " << p_e->data; 
			last_pos=p_e->pos; 
			p_e = p_e->next; 
		} 
		for (i=last_pos+1;i<v.dimension;++i) out << " " << "0"; 

		return out; 
	}

}