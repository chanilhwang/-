/* Start Header -------------------------------------------------------------
--
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ConvolutePNG.cpp
Purpose: Given a source PNG file, will generate all other convolution patterns and save them to png files.
Language: C++
Platform: Visual Studio 15.8.2 w/ C++17, g++-8 (Ubuntu 8.2.0-1ubuntu2~18.04) 8.2.0
Project: c.hwang_cs180_assignment4
Author: c.hwang
Creation date: 11/19/2018
- End Header ----------------------------------------------------------------
*/

#define SMALLWIDTH 200
#define SMALLHEIGHT 200

#include "ConvolutePNG.hpp"
#include "ColorGrid.hpp"
#include "ConvolutionType.hpp"
#include <future>
#include <iostream>

namespace
{
    CS180::ColorGrid convolute(const CS180::ColorGrid& source_grid, const CS180::ConvolutionMatrix convolution);
}

namespace CS180
{
    void generate_convolutions_and_their_png_files(const std::filesystem::path& source_png_file, const std::filesystem::path& destination_folder)
    {
        // TODO: for each convolution type generate a new image in it's own thread
        using namespace std::string_literals;
        using namespace std;
        const ColorGrid source_grid = load_color_grid_from_png_file_path(source_png_file.generic_string());

        // If it is small enough
        if (source_grid.height <= SMALLHEIGHT && source_grid.width <= SMALLWIDTH)
        {
            for (auto type = ConvolutionType::Identity; type < ConvolutionType::Length; ++type)
            {
                const auto colors_result = convolute(source_grid, CONVOLUTIONS[type]);
                auto destination_png = destination_folder / source_png_file.filename();
                destination_png.replace_extension("."s + CONVOLUTION_NAMES[type] + ".png"s);
                save_color_grid_to_png_file(colors_result, destination_png.generic_string());
            }
        }
        // otherwise
        else
        {
            std::vector<std::future<CS180::ColorGrid>> futures;
            for (auto type = ConvolutionType::Identity; type < ConvolutionType::Length; ++type)
            {
                futures.push_back(std::async(convolute, source_grid, CONVOLUTIONS[type]));
            }
            for (auto type = ConvolutionType::Identity; type < ConvolutionType::Length; ++type)
            {
                std::filesystem::path destination_png = destination_folder / source_png_file.filename();
                destination_png.replace_extension("."s + CONVOLUTION_NAMES[type] + ".png"s);
                save_color_grid_to_png_file(futures[type].get(), destination_png.generic_string());
            }
        }
    }
}
namespace
{
    /**
     * \brief Examine the 3x3 colors centered around (column,row). Multiply each pixel by some scalar value determined by the convolution type and
     * then average all the colors to get the new color.
     *
     * \param grid of pixel colors
     * \param color_row row center to read from
     * \param color_column column center to read from
     * \param convolution type of convolution to perform
     * \return the resulting color from doing the convolution
     */
    CS180::Color3D calculate_pixel(const CS180::ColorGrid& grid, unsigned color_row, unsigned color_column,
                                   const CS180::ConvolutionMatrix convolution)
    {
        CS180::Color3D sum{0, 0, 0};
        for (unsigned matrix_row = 0; matrix_row < 3; ++matrix_row)
        {
            for (unsigned matrix_column = 0; matrix_column < 3; ++matrix_column)
            {
                const auto offset_color_row = color_row + matrix_row - 1;
                const auto offset_color_column = color_column + matrix_column - 1;
                const auto color_scale = convolution[matrix_row][matrix_column];
                sum += grid(offset_color_row, offset_color_column) * color_scale;
            }
        }
        sum /= 9.0f;
        sum.ClampColorChannelsFromZeroToOne();

        return sum;
    }

    CS180::ColorGrid& calculate_pixels_in_range(CS180::ColorGrid* destination, const CS180::ColorGrid& grid, unsigned int begin, unsigned int end,
                                                const CS180::ConvolutionMatrix convolution)
    {
        for (unsigned int i = begin; i < end; i++)
        {
            const auto index_location = &grid.colors[i] - &grid.colors[0];
            const auto row = unsigned(index_location / grid.width);
            const auto column = unsigned(index_location % grid.width);

            if (column != 0 && column < grid.width - 1 && row != 0 && row < grid.height - 1)
            {
                const auto new_color = calculate_pixel(grid, row, column, convolution);
                (*destination)(row, column) = new_color;
            }
            else
            {
                (*destination)(row, column) = grid(row, column);
            }
        }

        return *destination;
    }

    CS180::ColorGrid convolute(const CS180::ColorGrid& source_grid, const CS180::ConvolutionMatrix convolution)
    {
        using namespace std;
        // TODO: generate destination over multiple threads
        CS180::ColorGrid destination(source_grid.width, source_grid.height);

        // async version
        unsigned int size = source_grid.height * source_grid.width;

        auto future0 = async(calculate_pixels_in_range, &destination, source_grid, 0, size / 4, convolution);
        auto future1 = async(calculate_pixels_in_range, &destination, source_grid, size / 4, size / 2, convolution);
        auto future2 = async(calculate_pixels_in_range, &destination, source_grid, size / 2, (size * 3) / 4, convolution);
        auto future3 = async(calculate_pixels_in_range, &destination, source_grid, (size * 3) / 4, size, convolution);

        future0.get();
        future1.get();
        future2.get();
        future3.get();

        return destination;
    }
}