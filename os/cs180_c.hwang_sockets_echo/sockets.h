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
// Author:    Rudy Castan <rudy.castan@digipen.edu>
// Creation date: 2018/11/08
#pragma once

#ifdef _WIN32
#include <WS2tcpip.h>   // almost everything is contained in one file.
#else                   // UNIX/Linux
#include <arpa/inet.h>  // contains all inet_* functions
#include <errno.h>      // contains the error functions
#include <fcntl.h>      // file control
#include <netdb.h>      // for DNS - gethostbyname()
#include <netinet/in.h> // IPv4 and IPv6 stuff
#include <sys/socket.h> // header containing socket data types and functions
#include <sys/types.h>  // header containing all basic data types and typedefs
#include <unistd.h>     // for gethostname()
#endif


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

    sock open_listen_socket(const char* service, Protocol protocol = Protocol::HostPreferred);
    sock open_client_socket(const char* host, const char* service);
    
    void close_socket(sock socket_handle);
}
