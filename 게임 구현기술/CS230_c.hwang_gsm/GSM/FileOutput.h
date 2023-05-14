/******************************************************************************/
/*!
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.

\file   	FileOutput.h
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

#ifndef FILEOUTPUT_H
#define FILEOUTPUT_H

void File_Created();
void File_Append(const char* string);
void File_Closed();

#endif //FILEOUTPUT_H