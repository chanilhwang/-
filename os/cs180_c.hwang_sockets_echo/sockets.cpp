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
// Creation date: 12/02/2018

#include "sockets.h"
#include <iostream>

namespace
{
    bool check_for_valid_address(const char* host, const char* service, const addrinfo* hints, addrinfo** results);
}

namespace sockets
{
    sock open_listen_socket(const char* service, Protocol protocol)
    {
        sock listen_socket{BAD_SOCKET};
		addrinfo hints = {};
		addrinfo* results = nullptr;
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
		check_for_valid_address(host, service, &hints, &results);

		for (auto current_address = results; current_address != nullptr;
			current_address = current_address->ai_next)
		{
			auto s = socket(current_address->ai_family, current_address->ai_socktype, current_address->ai_protocol);
			if (s == listen_socket)
				continue;

			auto err = bind(s, current_address->ai_addr, current_address->ai_addrlen);
			if (err == 0)
			{
				listen_socket = s;
				bind(listen_socket, current_address->ai_addr, current_address->ai_addrlen);
				break;
			}
		}
		listen(listen_socket, 1024);
        return listen_socket;
    }

    sock open_client_socket(const char* host, const char* service)
    {
        sock client_socket{BAD_SOCKET};
		addrinfo hints = {};
		addrinfo* results = nullptr;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_ADDRCONFIG;
		getaddrinfo(host, service, &hints, &results);

		check_for_valid_address(host, service, &hints, &results);
		for(auto current_address = results; current_address != nullptr;
			current_address = current_address->ai_next)
		{
			auto s = socket(current_address->ai_family, current_address->ai_socktype, current_address->ai_protocol);
			if (s == client_socket)
				continue;
			
			auto err = connect(s, current_address->ai_addr, current_address->ai_addrlen);
			if (err == 0)
			{
				client_socket = s;
				break;
			}
		}
        return client_socket;
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
    bool check_for_valid_address(const char* host, const char* service, const addrinfo* hints, addrinfo** results)
    {
		auto error = getaddrinfo(host, service, hints, results);
		if (error != 0)
		{
			std::cerr << "getaddrinfo() error: " << gai_strerror(error) << '\n';
			return true;
		}
        return false;
    }
#ifdef _WIN32
    class WindowsSocketSystem
    {
    public:
        WindowsSocketSystem()
        {
			WSADATA wsaData;
			WSAStartup(MAKEWORD(2, 2), &wsaData);
        }
        ~WindowsSocketSystem()
        {
			WSACleanup();
        }
    };
    WindowsSocketSystem g_system;
#endif
}