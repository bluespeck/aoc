#include <cassert>
#include <array>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "outf.h"

using Input = std::vector<std::string>;

Input read_input()
{
    Input input;
    while (std::cin)
    {
        std::string line;
        std::getline(std::cin, line);
        if (line.size())
        {
            input.push_back(line);
        }
    }
    return input;
}

void solve_1(const Input& _input)
{
    outf("\nPart {}: ", 1);
    bool something_moved = false;
    auto input = _input;
    int32_t step = 0;
    outf("Initial state:\n");
    for (size_t i = 0; i < input.size(); i++)
    {
        outf("{}\n", input[i]);
    }
    outf("\n");

    std::vector<std::vector<bool>> can_move;
    can_move.resize(input.size());
    for (size_t i = 0; i < can_move.size(); i++)
    {
        can_move[i].resize(input[i].size(), false);
    }

    do
    {
        something_moved = false;
        step++;
        for (size_t i = 0; i < input.size(); i++)
        {
            for (size_t j = 0; j < input[i].size(); j++)
            {
                can_move[i][j] = false;
            }
        }

        for (size_t i = 0; i < input.size(); i++)
        {
            for (size_t j = 0; j < input[i].size(); j++)
            {
                if (input[i][j] == '>' && input[i][(j + 1) % input[i].size()] == '.')
                {
                    can_move[i][j] = true;
                }
            }
        }
        for (size_t i = 0; i < input.size(); i++)
        {
            for (size_t j = 0; j < input[i].size(); j++)
            {
                if (can_move[i][j])
                {
                    std::swap(input[i][j], input[i][(j + 1) % input[i].size()]);
                    something_moved = true;
                }
            }
        }
        
        for (size_t i = 0; i < input.size(); i++)
        {
            for (size_t j = 0; j < input[i].size(); j++)
            {
                can_move[i][j] = false;
            }
        }

        for (size_t j = 0; j < input[0].size(); j++)
        {
            for (size_t i = 0; i < input.size(); i++)
            {
                if (input[i][j] == 'v' && input[(i + 1) % input.size()][j] == '.')
                {
                    can_move[i][j] = true;
                }
            }
        }

        for (size_t j = 0; j < input[0].size(); j++)
        {
            for (size_t i = 0; i < input.size(); i++)
            {
                if (can_move[i][j])
                {
                    std::swap(input[(i + 1) % input.size()][j], input[i][j]);
                    something_moved = true;
                }
            }
        }

        outf("After step {}:\n", step);
        for (size_t i = 0; i < input.size(); i++)
        {
            outf("{}\n", input[i]);
        }
        outf("\n");

    }
    while (something_moved);
    outf("{}", step);

}


int main()
{
    auto input = read_input();
    solve_1(input);
    
    return 0;
}
