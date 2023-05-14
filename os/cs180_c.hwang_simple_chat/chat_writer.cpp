// Copyright (C) 2018 DigiPen Institute of Technology.
// Reproduction or disclosure of this file or its contents without the prior
// written consent of DigiPen Institute of Technology is prohibited.
//
// File Name: client_writer.cpp
// Purpose:   Defines a networked client to write chats
// Language:  C++
// Platform:  Visual Studio 15.8.2 w/ C++17, g++ (Ubuntu 7.3.0-16ubuntu3) 7.3.0
// Project:   CS180 Operating Systems
// Author:   c.hwang
// Creation date: 2018/12/01
#include "sockets.h"
#include <array>
#include <string>
using namespace std;
int main(int argc, char const* argv[])
{
	if (!(argc == 4 || argc == 3))
	{
		cerr << "usage: " << argv[0] << " <server ip address> <port or service name> (<nickname>)\n";
		return 1;
	}
	const char* const server = argv[1];
	const char* const port = argv[2];
	const sockets::sock writer_socket = sockets::open_writer_socket(server, port);
	if (writer_socket == sockets::BAD_SOCKET)
		return 1;
	send(writer_socket, "c", 1, 0);
	string client_name = {};
	if (argc == 3) {
		cout << "Please enter in your client_name: ";
		getline(cin, client_name);
	}
	else
		client_name = argv[3];
	if (client_name == "")
	{
		cout << "invalid nickname";
		return 1;
	}
	else
	{
		string init = client_name + " has joined the chatroom!";
		send(writer_socket, init.data(), init.size(), 0);
	}
	string input;
	while (true)
	{
		getline(cin, input);
		if (input.empty())
			break;
		string sent_message = client_name + ">" + input;
		if (input == "quit")
		{
			sent_message = client_name + " has left the chat room!";
			send(writer_socket, sent_message.data(), sent_message.size(), 0);
			break;
		}
		else
		{
			const int send_bytes = send(writer_socket, sent_message.data(), sent_message.size(), 0);
			if (send_bytes <= 0)
			{
				if (send_bytes < 0)
				{
					cerr << "Failed to send message\n";
				}

				break;
			}
		}
	}
	sockets::close_socket(writer_socket);
	return 0;
}