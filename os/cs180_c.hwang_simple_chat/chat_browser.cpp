// Copyright (C) 2018 DigiPen Institute of Technology.
// Reproduction or disclosure of this file or its contents without the prior
// written consent of DigiPen Institute of Technology is prohibited.
//
// File Name: client_browser.cpp
// Purpose:   Defines a networked client to browsing
// Language:  C++
// Platform:  Visual Studio 15.8.2 w/ C++17, g++ (Ubuntu 7.3.0-16ubuntu3) 7.3.0
// Project:   CS180 Operating Systems
// Author:    c.hwang
// Creation date: 2018/12/01

#include "sockets.h"
#include <array>
#include <string>

using namespace std;

int main(int argc, char const* argv[])
{
	if (argc != 3)
	{
		cerr << "usage: " << argv[0] << " <server ip address> <port or service name>\n";
		return 1;
	}
	const char* const server = argv[1];
	const char* const port = argv[2];
	const sockets::sock writer_socket = sockets::open_writer_socket(server, port);
	if (writer_socket == sockets::BAD_SOCKET)
		return 1;
	array<char, 1024> recv_buffer{};
	send(writer_socket, "b", 1, 0);
	while (true)
	{
		int bytes_to_receive = 0;
		int	current_bytes;
		current_bytes = recv(writer_socket, recv_buffer.data(), recv_buffer.size(), 0);
		bytes_to_receive += current_bytes;
		if (current_bytes > 0)
		{
			recv_buffer[current_bytes] = 0;
			cout << recv_buffer.data();
			cout << "\n";
		}
		else
		{
			if (bytes_to_receive < 0)
				cerr << "failed to receive message\n";
			break;
		}
	}
	sockets::close_socket(writer_socket);
	return 0;
}