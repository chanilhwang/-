/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ellipse2d.cpp
Purpose: drawing ellipse
Language: c++
Platform: visual studio 17, window 64bit
Project: c.hwang_cs200_a2
Author: Hwang Chan IL, c.hwang, 5415279
Creation date: 10/18/2018
- End Header ----------------------------------------------------------------*/

Description
1. what has been implemented
	: To draw ellipse, I use midpoint algorithm and second order difference. I divide into two section:
	 the part where the slope of the tangent is greater than 1 and the part between 0 and 1. And then I draw
	Quadrant 2,3,4 with same variable x and y.

Portion of the assignment that is incomplete
1. only integer calculations
	: I can't explain why fractional portion is always same in non-programming portion.