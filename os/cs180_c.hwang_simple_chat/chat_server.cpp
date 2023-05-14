// Copyright (C) 2018 DigiPen Institute of Technology.
// Reproduction or disclosure of this file or its contents without the prior
// written consent of DigiPen Institute of Technology is prohibited.
//
// File Name: client_server.cpp
// Purpose:   Defines a client server program.
// Language:  C++
// Platform:  Visual Studio 15.8.2 w/ C++17, g++ (Ubuntu 7.3.0-16ubuntu3) 7.3.0
// Project:   CS180 Operating Systems
// Author:  c.hwang
// Creation date: 2018/12/01

#include "sockets.h"
#include <algorithm>
#include <array>
#include <vector>
#ifdef _WIN32
#include <thread>
#elif __linux__
#include <pthread.h>
#endif
using namespace std;

vector<sockets::sock> vector_for_browsers;

bool quit_message(const array<char, 1024>& data)
{
	using std::cbegin;
	using std::cend;
	constexpr char quit_string[] = "quit";
	const auto location = search(cbegin(data), cend(data), cbegin(quit_string), cend(quit_string));
	return (location != cend(data)) ;
}

void send_data(sockets::sock data_socket, const char* data, int size)
{
	int zero_byte = 0;
	do {
		int check_for_bytes =
			send(data_socket, data + zero_byte, size - zero_byte, 0);
		if (check_for_bytes <= 0)
			return;
		zero_byte += check_for_bytes;
	} while (zero_byte < size);
}

void handle_wirter(sockets::sock data_socket, bool& check_for_child, string& log_of_chat)
{
	array<char, 1024> recv_buffer = {};
	bool client_active = true;
	while (client_active)
	{
		auto recv_bytes = recv(data_socket, recv_buffer.data(), recv_buffer.size() - 1, 0);
		if (recv_bytes <= 0)
		{
			return;
		}
		recv_buffer[recv_bytes] = 0;
		for (auto it = vector_for_browsers.begin(); it != vector_for_browsers.end(); it++)
		{
			send(*it, recv_buffer.data(), recv_buffer.size() - 1, 0);
		}
		log_of_chat.append(recv_buffer.data());
		log_of_chat.append("\n");
		client_active = !quit_message(recv_buffer);
	}
	check_for_child = false;
}

void print_client_info(sockaddr_storage client_address, socklen_t size_of_socket)
{
	constexpr int NameBufLen = 1024;
	array<char, NameBufLen> client_hostname{};
	array<char, NameBufLen> client_port{};
	const auto psockaddr_info = reinterpret_cast<sockaddr*>(&client_address);

	getnameinfo(psockaddr_info, size_of_socket, client_hostname.data(), NameBufLen, client_port.data(), NameBufLen, 0);
	cout << "Connected to (" << client_hostname.data() << ", " << client_port.data() << ") / ";

	getnameinfo(psockaddr_info, size_of_socket, client_hostname.data(), NameBufLen, client_port.data(), NameBufLen, NI_NUMERICHOST);
	cout << "Connected to (" << client_hostname.data() << ", " << client_port.data() << ")\n\n";
}

int main(int argc, char const* argv[])
{
	if (argc != 2)
	{
		cerr << "usage: " << argv[0] << " <port or service name>\n";
		return 1;
	}

	const char* port = argv[1];
	const sockets::sock server_socket = sockets::open_server_socket(port, sockets::Protocol::IPv4);
	sockaddr_storage client_address = {};
	socklen_t size_of_socket = sizeof(client_address);
	bool check_for_child = true;
	string log_of_chat;

	while (check_for_child)
	{
		const sockets::sock data_socket =
			accept(server_socket, reinterpret_cast<sockaddr*>(&client_address),
				&size_of_socket);
		if (data_socket == sockets::BAD_SOCKET)
		{
			cout << "Bad Socket" << endl;
			break;
		}
		array<char, 1024> check_for_client_type = {};
		recv(data_socket, check_for_client_type.data(), 1, 0);
		auto handle_client = [&]()
		{
			print_client_info(client_address, size_of_socket);
			handle_wirter(data_socket, check_for_child, log_of_chat);
			sockets::close_socket(data_socket);
		};
#ifdef _WIN32
		if (check_for_client_type[0] == 'c')
		{
			thread t(handle_client);
			t.detach();
		}
#elif __linux__
		if (check_for_client_type[0] == 'c')
		{
			pid_t pid;
			pid = fork();
			if(pid < 0)
			{
				return 1;
			}
			else if(pid == 0)
			{
				handle_client();
			}
		}
#endif
		else if (check_for_client_type[0] == 'b')
		{
			print_client_info(client_address, size_of_socket);
			vector_for_browsers.push_back(data_socket);
			send(data_socket, log_of_chat.data(), log_of_chat.size() - 1, 0);
		}
	}
	sockets::close_socket(server_socket);
	return 0;
}