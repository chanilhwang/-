// Copyright (C) 2018 DigiPen Institute of Technology.
// Reproduction or disclosure of this file or its contents without the prior
// written consent of DigiPen Institute of Technology is prohibited.
//
// File Name: echoclient.cpp
// Purpose:   Defines a networked client to handle an echo server,
// Language:  C++
// Platform:  Visual Studio 15.8.2 w/ C++17, g++ (Ubuntu 7.3.0-16ubuntu3) 7.3.0
// Project:   CS180 Operating Systems
// Author:    c.hwang
// Creation date: 12/02/2018

#include "sockets.h"
#include <array>
#include <iostream>
#include <string>

int main(int argc, char const* argv[])
{
    if (argc != 3)
    {
        std::cerr << "usage: " << argv[0] << " <server ip address> <port or service name>\n";
        return 1;
    }

    const char* const server_host = argv[1];
    const char* const port = argv[2];

    const sockets::sock client_socket = sockets::open_client_socket(server_host, port);

    if (client_socket == sockets::BAD_SOCKET)
        return 1;

    std::string input_line;
    std::array<char, 1024> receive_buffer{};
    while (true)
    {
        std::getline(std::cin, input_line);
        if (input_line.empty())
            break;

        // TODO call send() to send the input_line to the server
        const int bytes_sent = send(client_socket, input_line.data(), input_line.size(), 0);

        if (bytes_sent <= 0)
        {
            if (bytes_sent < 0)
                std::cerr << "Failed to send message\n";
            break;
        }
        std::cout << "\n-----------------------------------\n";
        int bytes_received = 0;
        int current_bytes_received;
        do
        {
            // TODO call recv() to recieve the response from the server
            current_bytes_received = recv(client_socket, receive_buffer.data(), receive_buffer.size(), 0);

            bytes_received += current_bytes_received;
            if (current_bytes_received > 0)
            {
                receive_buffer[current_bytes_received] = 0;
                std::cout << receive_buffer.data();
            }
        } while (bytes_received < bytes_sent && current_bytes_received > 0);
        std::cout << "\n\n\n";

        if (bytes_received <= 0)
        {
            if (bytes_received < 0)
                std::cerr << "Failed to receive message\n";
            break;
        }

        if (input_line == "quit")
            break;
    }

    sockets::close_socket(client_socket);
    return 0;
}