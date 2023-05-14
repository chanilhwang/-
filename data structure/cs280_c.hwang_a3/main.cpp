/*------------------------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: main.cpp
Purpose: to test the function that translate morses code into word
Project: assignment 3
Author: c.hwang
Created: 2019/04/17
------------------------------------------------------------------------*/
#include <iostream>
#include <string>
#include "List.h"
#include "MorseCode.h"
#include "Dictionary.h"

TranslateMorseCode trans;
Dictionary dic;
std::string big = "big";
std::string morseofbig = trans.ToMorseCode(big);

void OutputAllValidWords(std::string input, std::string output) {
	int length = input.length();
	for (int i = 0; i < length; i++) {
		std::string letter = input.substr(0, i + 1);
		if (!trans.IsLetter(letter)) {
			continue;
		}
		OutputAllValidWords(input.c_str() + (i + 1), output + trans.GetLetter(letter));
	}
	//cout all of possible words or words which are translated all morse codes
	if (length == 0 && dic.IsWord(output)) {
		std::cout << output << std::endl;
	}
}

void OutputAllValidWords(std::string input) {
	OutputAllValidWords(input, "");
}

int main()
{
	std::cout << big << std::endl;
	std::cout << morseofbig << std::endl;

	OutputAllValidWords(morseofbig);

	return 0;
}