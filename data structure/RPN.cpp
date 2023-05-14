/*------------------------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: RPN.cpp
Purpose: to convert normal equation to rpn and evaluate rpn
Project: rpn lab
Author: c.hwang
Created: 2019/05/03
------------------------------------------------------------------------*/
#include <string>
#include <iostream>
#include "RPN.h"

int Priority(char token) {
	switch(token) {
	case '*':
	case '/':
		return 2;
	case '+':
	case '-':
		return 1;
	default:
		return 0;
	}
}

template<typename T>
class Stack
{
public:
	Stack(int num = 3) {
		value = new T[num];
		top = 0;
		max = num;
	}
	~Stack() {
		delete[] value;
	}
	void resize(int n);
	void push_back(char c);
	void pop_back();
	char back() const;
	int size() const;
	T& operator[](int i);

private:
	T* value;
	int max;
	int top;
};

template<typename T>
void Stack<T>::resize(int n)
{
	T* temp = new T[n];
	for (int i = 0; i < top; i++)
	{
		temp[i] = value[i];
	}
	if (max >= 2)
		delete[] value;
	else
		delete value;
	value = temp;
}

template<typename T>
void Stack<T>::push_back(char c)
{
	if (max == top) {
		resize(max + 1);
	}
	value[top] = c;
	top++;
}

template<typename T>
void Stack<T>::pop_back()
{
	if (top <= 0)
		return;
	top--;
}

template<typename T>
char Stack<T>::back() const
{
	if (size() == 0) {
		return NULL;
	}
	return value[top - 1];
}

template<typename T>
int Stack<T>::size() const
{
	return top;
}

template<typename T>
T & Stack<T>::operator[](int i)
{
	return value[i];
}

int EvaluateRPN(std::string const& postfix) {
	Stack<char> result;
	for (auto& c : postfix)
	{
		switch (Priority(c))
		{
		case 0:
			result.push_back(c);
			break;
		case 1:
		case 2:
			int temp = result.back() - '0';
			result.pop_back();
			if (c == '+')
				temp = result.back() - '0' + temp;
			else if (c == '-')
				temp = result.back() - '0' - temp;
			else if (c == '*')
				temp = (result.back() - '0') * temp;
			else if (c == '/')
				temp = (result.back() - '0') / temp;
			result.pop_back();
			result.push_back(temp + '0');
		}
	}
	return result.back() - '0';
}

std::string StandardToRPN(std::string const & infix) {
	Stack<char> oper(4);
	std::string result;

	for (auto& c : infix) {
		if (Priority(c) == 0) {
			if (c == '(')
			{
				oper.push_back(NULL);
			}
			else if (c == ')')
			{
				int size = oper.size();
				for (int i = 0; i < size; i++)
				{
					if (oper.back() == NULL)
					{
						break;
					}
					result.push_back(oper.back());
					oper.pop_back();
				}
				oper.pop_back();
			}
			else
			{
				result.push_back(c);
			}
			continue;
		}
		else {
			if (oper.back() == NULL) {
				oper.push_back(c);
			}
			else if (Priority(c) > Priority(oper.back())) {
				oper.push_back(c);
			}
			else if(Priority(c) <= Priority(oper.back())) {
				result.push_back(oper.back());
				oper.pop_back();
				oper.push_back(c);
			}
			else {
				oper.push_back(c);
			}
		}
	}

	int size = oper.size();
	for (int i = 0; i < size; i++)
	{
		result.push_back(oper.back());
		oper.pop_back();
	}

	return result;
}
