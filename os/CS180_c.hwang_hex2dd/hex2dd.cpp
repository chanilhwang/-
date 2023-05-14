/* Start Header -------------------------------------------------------------
--
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: hex2dd.cpp
Purpose: convert hexadecimal to dotted decimal
Language: c++
Platform: windows10 64bit / visual studio 2017
Project: c.hwang_cs180_lab
Author:c.hwang
Creation date: 11/13/2018
- End Header ----------------------------------------------------------------
*/
#include <iostream>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

int main(int argc, char **argv)
{
	if(argc > 1) {
		if(*(argv[1] + 1) == 'x')
		{
			char buf[20];
			struct in_addr addr;
			addr.s_addr = inet_addr(argv[1]);
		
			std::cout << inet_ntop(AF_INET, &addr.s_addr, buf, sizeof(buf)) << '\n';
		}
		else {
			std::cout << "This is not hex number!\n";
			return 0;
		}
	}
	return 0;
}
