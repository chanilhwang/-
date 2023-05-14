#pragma once
#include <string>
#include <iostream>

class RopeTree
{
public:
	class RopeTreeNode
	{
	public:
		RopeTreeNode() = delete;
		RopeTreeNode(unsigned int new_weight);
		RopeTreeNode(const std::string& str);
		RopeTreeNode(RopeTreeNode* lhs, RopeTreeNode* rhs);
		~RopeTreeNode();

		char Find(int num) const;
		void Split(int num);
		friend std::ostream& operator<<(std::ostream& os, const RopeTreeNode* node);

		RopeTreeNode* left;
		RopeTreeNode* right;
	private:
		unsigned int weight;
		std::string string;
	};

	RopeTree();
	RopeTree(const std::string& str);
	~RopeTree();
	char operator[](int num) const;
	RopeTree& operator+=(RopeTree& rhs);
	RopeTree& operator+=(RopeTree&& rhs);
	friend std::ostream& operator<<(std::ostream& os, const RopeTree& rope);

	char Find(int num) const;
	bool Empty() const;

private:
	RopeTreeNode* root;
};