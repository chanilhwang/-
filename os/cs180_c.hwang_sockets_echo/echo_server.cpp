// Copyright (C) 2018 DigiPen Institute of Technology.
// Reproduction or disclosure of this file or its contents without the prior
// written consent of DigiPen Institute of Technology is prohibited.
//
// File Name: echoserver.cpp
// Purpose:   Defines an echo server program.
// Language:  C++
// Platform:  Visual Studio 15.8.2 w/ C++17, g++ (Ubuntu 7.3.0-16ubuntu3) 7.3.0
// Project:   CS180 Operating Systems
// Author:    c.hwang
// Creation date: 12/02/2018

#include "sockets.h"
#include <algorithm>
#include <array>
#include <iostream>

bool echo_to_client_and_check_if_should_continue(sockets::sock data_socket);
void print_client_information(sockaddr_storage client_address, socklen_t socket_address_storage_size);

int main(int argc, char const* argv[])
{
    if (argc != 2)
    {
        std::cerr << "usage: " << argv[0] << " <port or service name>\n";
        return 1;
    }

    const char* port = argv[1];
    const sockets::sock listen_socket = sockets::open_listen_socket(port, sockets::Protocol::IPv4);
    socklen_t socket_address_storage_size = sizeof(sockaddr_storage);
    bool should_run = true;
    while (should_run)
    {
        sockaddr_storage client_address = {};
		const sockets::sock new_client_data_socket = accept(listen_socket, reinterpret_cast<sockaddr*>(&client_address),&socket_address_storage_size);

        if (new_client_data_socket == sockets::BAD_SOCKET)
        {
            continue;
        }

        print_client_information(client_address, socket_address_storage_size);
        should_run = echo_to_client_and_check_if_should_continue(new_client_data_socket);
    }

    sockets::close_socket(listen_socket);
    return 0;
}

bool does_data_contain_kill_message(const std::array<char, 512>& data)
{
    using std::cbegin;
    using std::cend;
    constexpr char kill_string[] = "kill server";
    const auto location = std::search(cbegin(data), cend(data), cbegin(kill_string), cend(kill_string));
    return location != cend(data);
}

bool echo_to_client_and_check_if_should_continue(sockets::sock data_socket)
{
    std::array<char, 512> receive_buffer{};
    bool continue_server = true;
    bool client_is_active = true;
    while (client_is_active)
    {
        const auto bytes_received = recv(data_socket, receive_buffer.data(), receive_buffer.size(), 0);

        if (bytes_received <= 0)
            break;

        receive_buffer[bytes_received] = 0;
        std::cout << receive_buffer.data() << "\n\t";
        std::cout << "server received " << bytes_received << " bytes\n";

        const auto bytes_sent = send(data_socket, receive_buffer.data(), receive_buffer.size(), 0);
        if (continue_server)
        {
            continue_server = !does_data_contain_kill_message(receive_buffer);
            if (!continue_server)
            {
                std::cout << "*** kill server message received! Server will shutdown after client connection closes.\n\n";
            }
        }
        client_is_active = (bytes_sent > 0);
    }
    sockets::close_socket(data_socket);
    return continue_server;
}

void print_client_information(sockaddr_storage client_address, socklen_t socket_address_storage_size)
{
    constexpr int NameBufferLength = 512;
    std::array<char, NameBufferLength> client_hostname{};
    std::array<char, NameBufferLength> client_port{};
    const auto psocketaddress_information = reinterpret_cast<sockaddr*>(&client_address);
	getnameinfo(psocketaddress_information, socket_address_storage_size, client_hostname.data(), NameBufferLength, client_port.data(), NameBufferLength, 0);
    std::cout << "Connected to (" << client_hostname.data() << ", " << client_port.data() << ") / ";
	getnameinfo(psocketaddress_information, socket_address_storage_size, client_hostname.data(), NameBufferLength, client_port.data(), NameBufferLength, NI_NUMERICHOST);
    std::cout << "(" << client_hostname.data() << ", " << client_port.data() << ")\n\n";
}