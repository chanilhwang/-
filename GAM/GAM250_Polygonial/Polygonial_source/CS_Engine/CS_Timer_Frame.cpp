/**
 * @file CS_Timer_Frame.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief For Timer Frame
 * @version 0.1
 * @date 2018-12-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "CS_Timer_Frame.hpp"
#include <chrono>


namespace TimeFrameInfo
{
	// Type aliases to make accessing nested type easier
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1> >;
	std::chrono::time_point<clock_t> timestamp= clock_t::now();
	float delta_time;
	float frame_time;
	float frame_count;
	float fps;
}


void CS_Timer_Frame::Reset()
{
	TimeFrameInfo::timestamp = TimeFrameInfo::clock_t::now();
}

double CS_Timer_Frame::GetElapsedSeconds()
{
	return std::chrono::duration_cast<TimeFrameInfo::second_t>(TimeFrameInfo::clock_t::now() - TimeFrameInfo::timestamp).count();
}

void CS_Timer_Frame::Update()
{
	TimeFrameInfo::delta_time = (float)GetElapsedSeconds();
	Reset();
	++TimeFrameInfo::frame_count;
	TimeFrameInfo::frame_time += TimeFrameInfo::delta_time;
	if (TimeFrameInfo::frame_time >= 1.0)
	{
		TimeFrameInfo::fps = TimeFrameInfo::frame_count / TimeFrameInfo::frame_time;
		TimeFrameInfo::frame_count = 0;
		TimeFrameInfo::frame_time = 0;
	}
}

float CS_Timer_Frame::GetDt()
{
	return TimeFrameInfo::delta_time;
}

int CS_Timer_Frame::GetFPS()
{
	return (int) TimeFrameInfo::fps;
}
