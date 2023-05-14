#include "RopeTree.h"

RopeTree::RopeTreeNode::RopeTreeNode(unsigned int new_weight)
	:left(nullptr), right(nullptr),
	weight(new_weight)
{}

RopeTree::RopeTreeNode::RopeTreeNode(const std::string& str) 
	: string(str),
	left(nullptr), right(nullptr)
{
	constexpr int max_length = 3;
	weight = static_cast<unsigned int>(string.length());
	if (weight > max_length) {
		Split(weight / 2);
	}
}

RopeTree::RopeTreeNode::RopeTreeNode(RopeTreeNode* lhs, RopeTreeNode* rhs)
	: left(lhs), right(rhs)
{
	weight = lhs->weight + rhs->weight;
}

RopeTree::RopeTreeNode::~RopeTreeNode() {
	if (left)
		delete left;
	if (right)
		delete right;
}

char RopeTree::RopeTreeNode::Find(int num) const {
	if (!string.empty()) {
		return string[num];
	}
	else {
		if (weight > static_cast<unsigned int>(num))
			if (left)
				return left->Find(num);
			else
				return 0;
		else
			if (right)
				return right->Find(num - weight);
			else
				return 0;
	}
}

void RopeTree::RopeTreeNode::Split(int num) {
	if (weight >= static_cast<unsigned int>(num)) {
		std::string substring = string.substr(0, num);
		left = new RopeTreeNode(substring);
		substring = string.substr(num, weight);
		right = new RopeTreeNode(substring);
		weight = num;
		string.clear();
	}
}

std::ostream & operator<<(std::ostream & os, const RopeTree::RopeTreeNode * node)
{
	if (!node->string.empty()) {
		os << node->string;
	}
	else {
		if(node->left != nullptr)
			os << node->left;
		if(node->right != nullptr)
			os << node->right;
	}
	return os;
}

RopeTree::RopeTree() : root(nullptr) {

}

RopeTree::RopeTree(const std::string& str) {
	root = new RopeTreeNode(static_cast<unsigned int>(str.length()));
	root->left = new RopeTreeNode(str);
}

RopeTree::~RopeTree() {
	if (!Empty()) {
		delete root;
	}
}

char RopeTree::operator[](int num) const {
	return Find(num);
}

RopeTree& RopeTree::operator+=(RopeTree& rhs) {
	RopeTreeNode* new_root = new RopeTreeNode(root, rhs.root);
	root = new_root;
	rhs.root = nullptr;
	return *this;
}

RopeTree& RopeTree::operator+=(RopeTree&& rhs) {
	RopeTreeNode* new_root = new RopeTreeNode(root, rhs.root);
	root = new_root;
	rhs.root = nullptr;
	return *this;
}

std::ostream& operator<<(std::ostream& os, const RopeTree& rope) {
	os << rope.root;

	return os;
}

char RopeTree::Find(int num) const {
	if (!Empty())
		return root->Find(num);
	else
		return 0;
}

bool RopeTree::Empty() const {
	return root == nullptr;
}