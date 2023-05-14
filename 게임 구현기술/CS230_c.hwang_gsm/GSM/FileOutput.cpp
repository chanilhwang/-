/******************************************************************************/
/*!
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.

\file   	FileOutput.cpp
\author		Hwang Chan IL
\par		c.hwang
\par    	tktlssla803@gmail.com
\par    	CS230
\par    	Assignment_GSM
\date   	5/24/2017

Some of the functions will have to append a string to a file called “Output.txt”.
This “Output.txt” file should be created and opened in the system initialization function,
and it should be closed in the system exit function.
*/
/******************************************************************************/

#include "FileOutput.h"
#include <fstream>

namespace
{
	std::ofstream fout;
}

void File_Created()
{
	fout.open("Output.txt");
}

void File_Append(const char* string)
{
	fout << string << std::endl;
}

void File_Closed()
{
	fout.close();
}