/* Start Header -------------------------------------------------------------
--
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: main.cpp
Purpose: entry point for image manipulation
Language: C++
Platform: Visual Studio 15.8.2 w/ C++17, g++-8 (Ubuntu 8.2.0-1ubuntu2~18.04) 8.2.0
Project: c.hwang_cs180_assignment4
Author: c.hwang
Creation date: 11/19/2018
- End Header ----------------------------------------------------------------
*/

#ifdef __linux__
// Posix includes here
#include <unistd.h>

#elif defined _WIN32 || defined _WIN64
// Windows includes here
#include <windows.h>

#else
#error "unknown platform"

#endif

#include "ConvolutePNG.hpp"
#include <filesystem>
#include <future>
#include <iostream>
#include <vector>

namespace
{
    struct InputParameters
    {
        int statusCode = 0;
        std::vector<std::filesystem::path> pngPaths{};
        std::filesystem::path destinationPath;
    };
    InputParameters extract_input_from_command_line(int argc, char* argv[]);
}

int main(int argc, char* argv[])
{
    using namespace std;
    // https://en.cppreference.com/w/cpp/language/structured_binding
    auto [status_code, png_paths, destination_path] = extract_input_from_command_line(argc, argv);
    if (status_code != 0)
        return status_code;

    std::filesystem::path* first = &png_paths[0];
    int size = png_paths.size();

    if (size == 1)
    {
        CS180::generate_convolutions_and_their_png_files(*first, destination_path);
    }
    else if (size > 1)
    {
#ifdef __linux__
        pid_t* pids = new pid_t[size - 1];

        for (int runningProcess = 0; runningProcess < size - 1; ++runningProcess)
        {
            pids[runningProcess] = fork();
			if (pids[runningProcess] < 0)
			{
				cout << "fork failed" << endl;
			}
			else if (pids[runningProcess] == 0)
			{
				CS180::generate_convolutions_and_their_png_files(png_paths.at(runningProcess + 1), destination_path);
				sleep(10);
				exit(0);
			}
        }

		CS180::generate_convolutions_and_their_png_files(*first, destination_path);

		delete [] pids;

        // windows
#elif defined _WIN32 || defined _WIN64
		CS180::generate_convolutions_and_their_png_files(*first, destination_path);

        STARTUPINFO si;
        std::vector<PROCESS_INFORMATION> pi(size - 1);

        memset((void*)&si, 0x00, sizeof(si));

        for (int i = 0; i < size - 1; i++)
            memset((void*)&pi[i], 0x00, sizeof(pi[i]));

        for (int i = 0; i < size - 1; i++)
        {
            char command[100];
            strcpy_s(command, sizeof(command), argv[0]);
            strcat_s(command, sizeof(command), " ");
            strcat_s(command, sizeof(command), argv[i + 2]);
            if (!CreateProcessA(argv[0], command, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi[i]))
            {
                cout << "creating process failed" << endl;
                return -1;
            }
        }

        for (int i = 0; i < size - 1; i++)
            WaitForSingleObject(pi[i].hProcess, INFINITE);

        cout << "Process End" << endl;

        for (int i = 0; i < size - 1; i++)
        {
            CloseHandle(pi[i].hProcess);
            CloseHandle(pi[i].hThread);
        }
#endif
    }
    else
    {
        cout << "invalid size" << endl;
    }

    return 0;
}

namespace
{
    void print_usage(char* argv[]) { std::cerr << "usage: " << argv[0] << " file_1.png file_2.png file_n.png [destination_folder]\n"; }

    InputParameters extract_input_from_command_line(int argc, char* argv[])
    {
        InputParameters input_values;
        input_values.destinationPath = std::filesystem::current_path();
        if (argc == 1)
        {
            print_usage(argv);
            input_values.statusCode = 1;
            return input_values;
        }

        for (int i = 1; i < argc; ++i)
        {
            auto current_input_path = std::filesystem::path{argv[i]};
            if (!current_input_path.has_extension())
            {
                if (i + 1 < argc)
                {
                    print_usage(argv);
                    input_values.statusCode = 1;
                    return input_values;
                }
                else
                {
                    if (!exists(current_input_path))
                    {
                        std::cerr << "Error: The following path does not exist:\n\t" << argv[i] << "\n";
                        input_values.statusCode = 2;
                        return input_values;
                    }
                    input_values.destinationPath = std::move(current_input_path);
                }
            }
            else
            {
                if (current_input_path.extension() != ".png")
                {
                    std::cerr << "Error: The following file name does not end with png:\n\t" << argv[i] << "\n";
                    input_values.statusCode = 3;
                    return input_values;
                }
                input_values.pngPaths.emplace_back(std::move(current_input_path));
            }
        }

        return input_values;
    }
}
