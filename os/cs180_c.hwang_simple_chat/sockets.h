// Copyright (C) 2018 DigiPen Institute of Technology.
// Reproduction or disclosure of this file or its contents without the prior
// written consent of DigiPen Institute of Technology is prohibited.
//
// File Name: sockets.h
// Purpose:   To have one header file to expose berkeley sockets, smooth out the platform subtle differences and to
//            provide helper functions that help you create common TCP/IP sockets.
// Language:  C++
// Platform:  Visual Studio 15.8.2 w/ C++17, g++ (Ubuntu 7.3.0-16ubuntu3) 7.3.0
// Project:   CS180 Operating Systems
// Author:    c.hwang
// Creation date: 2018/12/01
#pragma once

#ifdef _WIN32
#include <WS2tcpip.h>
#else
#include <arpa/inet.h>
#include <errno.h>     
#include <fcntl.h>     
#include <netdb.h>     
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include <unistd.h>    
#endif

#include <iostream>

namespace sockets
{
#ifdef _WIN32
	
	typedef SOCKET sock;
	constexpr sock BAD_SOCKET = INVALID_SOCKET;

#else
	using sock = int;
	constexpr sock BAD_SOCKET = ~0;
#endif

	enum class Protocol
	{
		IPv4,
		IPv6,
		HostPreferred
	};

	sock open_writer_socket(const char* host, const char* service);
	sock open_server_socket(const char* service, Protocol protocol);

	void close_socket(sock socket_handle);
}