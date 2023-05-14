// Copyright (C) 2018 DigiPen Institute of Technology.
// Reproduction or disclosure of this file or its contents without the prior
// written consent of DigiPen Institute of Technology is prohibited.
//
// File Name: sockets.cpp
// Purpose:   To implement the platform specifics needed for berkeley sockets.
// Language:  C++
// Platform:  Visual Studio 15.8.2 w/ C++17, g++ (Ubuntu 7.3.0-16ubuntu3) 7.3.0
// Project:   CS180 Operating Systems
// Author:    c.hwang
// Creation date: 2018/12/01
#include "sockets.h"
namespace
{
	bool check_for_valid_address(const char* host, const char* service, const addrinfo* hints, addrinfo** result);
}
namespace sockets
{
	sock open_server_socket(const char* service, Protocol protocol)
	{
		sock server_socket{ BAD_SOCKET };
		addrinfo hints = {};
		addrinfo *result = nullptr;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		hints.ai_family = AF_INET;
		hints.ai_protocol = (int)protocol;
		if (protocol == Protocol::HostPreferred)
			hints.ai_flags |= AI_ADDRCONFIG;
		char host[255];
		if(0 != gethostname(host, sizeof(host)))
		{
			std::cout << "gethostname failed" << std::endl;
			return 1;
		}
		else
			std::cout << "host name is : '" << host << "'" << std::endl;
		if (check_for_valid_address(host, service, &hints, &result))
		{
			for (auto address = result; address != nullptr; address = address->ai_next)
			{
				auto socket_temp = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
				if (socket_temp == server_socket)
					continue;
				auto check_error = bind(socket_temp, address->ai_addr, address->ai_addrlen);
				if (check_error == 0)
				{	
					server_socket = socket_temp;
					break;
				}
			}
			listen(server_socket, 1024);
		}
		return server_socket;
	}
	sock open_writer_socket(const char* host, const char* service)
	{
		sock writer_socket{ BAD_SOCKET };
		addrinfo hints = {};
		addrinfo* result = nullptr;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_ADDRCONFIG;
		if (check_for_valid_address(host, service, &hints, &result))
		{
			for (auto address = result; address != nullptr; address = address->ai_next)
			{
				auto socket_temp = socket(address->ai_family, address->ai_socktype, address->ai_protocol);

				if (socket_temp == writer_socket)
					continue;

				auto check_error = connect(socket_temp, address->ai_addr, address->ai_addrlen);

				if (check_error == 0)
				{
					writer_socket = socket_temp;
					break;
				}
			}
		}
		return writer_socket;
	}
	void close_socket(sock socket_handle)
	{
		#ifdef _WIN32
		closesocket(socket_handle);
		#else
		close(socket_handle);
		#endif

	}
}
namespace
{
	bool check_for_valid_address(const char* host, const char* service, const addrinfo* hints, addrinfo** result)
	{
		auto check_error = getaddrinfo(host, service, hints, result);
		
		if (check_error != 0)
		{
			std::cerr << "getaddrinfo() error : " << gai_strerror(check_error) << std::endl;
			return false;
		}
		return true;
	}
#ifdef _WIN32
	class Win32SocketSystem
	{
	public:
		Win32SocketSystem()
		{
			WSADATA wsaData;
			WSAStartup(MAKEWORD(2, 2), &wsaData);
		}
		~Win32SocketSystem()
		{
			WSACleanup();
		}
	};
	Win32SocketSystem g_system;
#endif
}