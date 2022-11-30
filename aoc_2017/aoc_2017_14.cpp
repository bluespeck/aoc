#include <cstdint>
#include "x64/../../outf.h"
#include <ranges>
#include <vector>
#include <iostream>
#include <cassert>
#include <unordered_map>
#include <sstream>

using input_t = std::vector<std::string>;

input_t read()
{
    std::string input_hash = "hwlqcszp";
    input_t input;
    input.resize(128);
    for (int i = 0; i < 128; i++)
    {
        input[i] = input_hash + "-" + std::to_string(i);
    }
    return input;
}

int64_t count_1_bits(int64_t num)
{
    int64_t count = 0;
    while (num)
    {
        if (num & 1)
            count++;
        num >>= 1;
    }
    return count;
}


void solve_1(const input_t& input)
{
    int64_t total_ones_count = 0;
    for (auto ii = 0; ii < 128; ii++)
    {
        using list_t = std::vector<int64_t>;
        list_t lengths;
        for (auto ch : input[ii])
        {
            lengths.push_back(static_cast<int64_t>(ch));
        }
        lengths.push_back(17);
        lengths.push_back(31);
        lengths.push_back(73);
        lengths.push_back(47);
        lengths.push_back(23);
        list_t list;
        for (size_t i = 0; i < 256; i++)
            list.push_back(i);

        size_t current_position = 0;
        size_t skip_size = 0;

        for (int count = 0; count < 64; count++)
        {
            for (auto length : lengths)
            {
                for (size_t i = 0; i < length / 2; i++)
                {
                    std::swap(list[(current_position + i) % list.size()], list[(current_position + (length - i - 1)) % list.size()]);
                }

                current_position = (current_position + length + skip_size++) % list.size();
            }
        }

        int64_t ones_count = 0;
        for (int i = 0; i < 16; i++)
        {
            int64_t xor16 = 0;
            for (int j = 0; j < 16; j++)
            {
                xor16 ^= list[i * 16 + j];
            }
            ones_count += count_1_bits(xor16);
        }
        total_ones_count += ones_count;
    }

    outf("\nNumber of used squares {}\n", total_ones_count);
}

void flood_fill(std::vector<std::string> &grid, int32_t i, int32_t j)
{
    if (i >= 0 && i < 128 && j >= 0 && j < 128 && grid[i][j] == '1')
    {
        grid[i][j] = '0';
        flood_fill(grid, i + 1, j);
        flood_fill(grid, i - 1, j);
        flood_fill(grid, i, j + 1);
        flood_fill(grid, i, j - 1);
    }
}

int32_t count_regions(std::vector<std::string>& grid)
{
    int32_t count = 0;

    while (true)
    {
        bool found = false;
        for (int i = 0; i < 128 && !found; i++)
        {
            for (int j = 0; j < 128; j++)
            {
                if (grid[i][j] == '1')
                {
                    count++;
                    flood_fill(grid, i, j);
                    found = true;
                    break;
                }
            }
        }
        if (!found)
            break;
    }
    return count;
}

void solve_2(const input_t& input)
{
    std::vector<std::string> grid;

    for (auto ii = 0; ii < 128; ii++)
    {
        grid.push_back("");
        
        using list_t = std::vector<int64_t>;
        list_t lengths;
        for (auto ch : input[ii])
        {
            lengths.push_back(static_cast<int64_t>(ch));
        }
        lengths.push_back(17);
        lengths.push_back(31);
        lengths.push_back(73);
        lengths.push_back(47);
        lengths.push_back(23);
        list_t list;
        for (size_t i = 0; i < 256; i++)
            list.push_back(i);

        size_t current_position = 0;
        size_t skip_size = 0;

        for (int count = 0; count < 64; count++)
        {
            for (auto length : lengths)
            {
                for (size_t i = 0; i < length / 2; i++)
                {
                    std::swap(list[(current_position + i) % list.size()], list[(current_position + (length - i - 1)) % list.size()]);
                }

                current_position = (current_position + length + skip_size++) % list.size();
            }
        }

        for (int i = 0; i < 16; i++)
        {
            int64_t xor16 = 0;
            for (int j = 0; j < 16; j++)
            {
                xor16 ^= list[i * 16 + j];
            }
        
            grid[ii] += std::format("{:08b}", xor16);
        }
        //outf("\n{}", grid[ii]);
    }

    outf("\nNumber of regions {}", count_regions(grid));

}

int main()
{
    input_t input = read();

    solve_1(input);
    solve_2(input);

    outf("\nDone.\n");
    return 0;
}