/* Start Header -------------------------------------------------------------
--
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: dd2hex.cpp
Purpose: convert dotted decimal to hexadecimal
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
		struct in_addr addr;
		
		inet_pton(AF_INET, argv[1], &addr.s_addr);
		std::cout << std::hex << "0x" << ntohl(addr.s_addr) << '\n';
	}
	return 0;
}