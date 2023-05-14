File Name: line2d.cpp
Purpose: line class implementation
Project: c.hwang_CS200_a1
Author: hwang chan il, c.hwang, tktlssla803@gmail.com
Creation date: 10/4/2018

Manual ) 
What has been implemented : DDA, Middlepoint algorithm
	DDA : 	Draw a line using the simple mathmetics equation y = mx + b
		Devide the code into three section : dx = 0, |m| > 1, 0 <= |m| <= 1
		In dx = 0, I draw a line with fixed x value and changing y value.
		In |m| > 1, draw a line based on y-axis
		In 0 <= |m| <= 1, draw a line based on x-axis
	MP algorithm :	Draw a line using the middle point of next pixel
			Devide the code into two section : |m| > 1, 0 <= |m| <= 1
			In |m| > 1, draw a line based on y-axis
			In 0 <= |m| <= 1, draw a line based on x-axis
Any modification : symmetrical version of DDA, MP algorithm
		After to determine if dy(if |m| > 1, dx) is odd or even,
		If dy is odd, draw line:P0P1 and line:P1P0 dy/2 times and draw the 		middlepoint of the line.
		If dy is even, just draw line:P0P1 and line:P1P0 dy/2 times
Not attempted  : 	Compared with sample.exe, my output using middlepoint algorithm is a 		little bit different.