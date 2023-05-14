/* Start Header -------------------------------------------------------------
--
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: statistics.cpp
Purpose: to implement statistics class
Language: c++
Platform: window10 64bit
Project: c.hwang_cs225_lab8
Author: c.hwang
Creation date: 12/02/2018
- End Header ----------------------------------------------------------------
*/
#include "statistics.h"
#include <numeric> //accumulate
#include <cmath> //sqrt
#include <iterator> //ostream_iterator, back_inserter
#include <iomanip> //setfill setw
#include <iostream> //cout

using namespace std;

Statistics::Statistics() : data() {}

double Statistics::Average() const
{
    return static_cast<double>(accumulate(data.begin(), data.end(), 0)) / Size();
}

unsigned int Statistics::Size() const
{
    return data.size();
}

int Statistics::Minimum() const
{
    return *min_element(data.begin(), data.end());
}

int Statistics::Maximum() const
{
    return *max_element(data.begin(), data.end());
}

double Statistics::Deviation() const
{
    double aver = Average();
    double size = static_cast<double>(Size());
	double result = 0;
	for_each(data.begin(), data.end(), [&result, &aver](int elem) {
		result += (elem - aver) * (elem-aver);
	});

	return sqrt(1 / size * (result));
}

std::map<int, int> Statistics::OccuresMoreThan(const int& condition_num) const
{
	map<int, int> map, result;

	for_each(data.begin(), data.end(), [&condition_num, &map](int elem) {
		if (map.find(elem) == map.end())
		{
			map[elem] = 1;
		}
		else
		{
			map[elem] += 1;
		}
	});

	for_each(map.begin(), map.end(), [&result, &condition_num](pair<int,int> elem) {
		if (elem.second > condition_num)
		{
			result[elem.first] = elem.second;
		}
	});

	return result;
}

std::vector<int> Statistics::Histogram(int num_of_bins, int beginning_of_range, int end_of_range)
{
	double bins_range = static_cast<double>(end_of_range - beginning_of_range) / num_of_bins;
	std::vector<int> result(num_of_bins);

	int i = 1;
	for_each(result.begin(), result.end(), [=, &i, &beginning_of_range, &bins_range](int& elem){
		elem = count_if(data.begin(), data.end(), [&i, &beginning_of_range, &bins_range](int elem) {
					return (elem < (beginning_of_range + i * bins_range))
						&& (elem >= (beginning_of_range + (i - 1) * bins_range));
				});
		i++;
	});

	return result;
}

//ExtraCredit Part
void Statistics::DrawHistogramH(int num_of_bins, int beginning_of_range, int end_of_range) const
{
	double bins_range = static_cast<double>(end_of_range - beginning_of_range) / num_of_bins;
	std::vector<int> result(num_of_bins);

	int i = 1;
	for_each(result.begin(), result.end(), [=, &i, &beginning_of_range, &bins_range](int& elem){
		elem = count_if(data.begin(), data.end(), [&i, &beginning_of_range, &bins_range](int elem) {
					return (elem < (beginning_of_range + i * bins_range))
						&& (elem >= (beginning_of_range + (i - 1) * bins_range));
				});
		i++;
	});
	
	vector<int> i_vector(num_of_bins);
	iota(i_vector.begin(), i_vector.end(), 0);
	for_each(i_vector.begin(), i_vector.end(), [&result](int& i){
		i = result.at(i);
		vector<int> j_vector(i);
		for_each(j_vector.begin(), j_vector.end(), [](int& j){
			j = 0;
			cout << "*";
		});
		cout << endl;
	});
}

void Statistics::DrawHistogramV(int num_of_bins, int beginning_of_range, int end_of_range) const
{
	double bins_range = static_cast<double>(end_of_range - beginning_of_range) / num_of_bins;
	vector<int> result(num_of_bins);

	int i = 1;
	for_each(result.begin(), result.end(), [=, &i, &beginning_of_range, &bins_range](int& elem){
		elem = count_if(data.begin(), data.end(), [&i, &beginning_of_range, &bins_range](int elem) {
					return (elem < (beginning_of_range + i * bins_range))
						&& (elem >= (beginning_of_range + (i - 1) * bins_range));
				});
		i++;
	});

	int result_max_element = *max_element(result.begin(), result.end());
	
	vector<int> i_vector(result_max_element);
	iota(i_vector.begin(), i_vector.end(), 0);
	for_each(i_vector.begin(), i_vector.end(), [&result, &result_max_element](int i){
		for_each(result.begin(), result.end(), [&i, &result_max_element](int elem) {
			if (elem >= result_max_element - i)
				cout << "*";
			else
				cout << " ";
		});
		cout << endl;
	});
}

std::ostream& operator<<( std::ostream& out, Statistics const& stat ) {
    std::copy( stat.data.begin(),  stat.data.end(),
               std::ostream_iterator<int>( out, " " ) );
    return out;
}

std::istream& operator>>( std::istream& in, Statistics & stat ) {
    std::copy( std::istream_iterator<int>(in),
               std::istream_iterator<int>(),
               std::back_inserter( stat.data ) );
    return in; 
}

