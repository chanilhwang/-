/* Start Header -------------------------------------------------------------
--
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: statistics.h
Purpose: statistics
Language: c++
Platform: window10 64bit
Project: c.hwang_cs225_lab8
Author: c.hwang
Creation date: 12/02/2018
- End Header ----------------------------------------------------------------
*/
#ifndef STATISTICS_H
#define STATISTICS_H
#include <vector>    //vector
#include <algorithm> //copy
#include <ostream>   //ostream
#include <istream>   //istream
#include <map>       //map
#include <functional>

class Statistics {
    private:
        typedef std::vector<int> ContainerType;
        ContainerType data;
    public:
        Statistics();
        template <typename T> Statistics( T const* b, T const* e );

        double Average() const;
        unsigned int Size() const;
        int Minimum() const;
        int Maximum() const;
        double Deviation() const;

		std::map<int, int> OccuresMoreThan(const int& num) const;
		template <typename Function>
		void RemoveIf(Function condition);
		std::vector<int> Histogram(int num_of_bins, int beginning_of_range, int end_of_range);
		void DrawHistogramH(int num_of_bins, int beginning_of_range, int end_of_range) const;
		void DrawHistogramV(int num_of_bins, int beginning_of_range, int end_of_range) const;

        friend std::ostream& operator<<( std::ostream& out, Statistics const& stat );
        friend std::istream& operator>>( std::istream& in, Statistics & stat );
};

template <typename Function>
void Statistics::RemoveIf(Function condition)
{
	data.erase(remove_if(data.begin(), data.end(), condition),
		data.end());
}

template <typename T>
Statistics::Statistics( T const* b, T const* e ) : data(e-b) {
    std::copy( b, e, data.begin() ); // we can use regular pointers to specify ranges!
}

#endif
