#include <cstdint>
#include "x64/../../outf.h"
#include <ranges>
#include <vector>
#include <iostream>
#include <cassert>
#include <unordered_map>

using input_t = std::string;

input_t read()
{
    input_t input;
    std::cin >> input;

    return input;
}

void solve_1(const input_t& input)
{
    std::string str_no_exclamation;
    // parse !
    for (size_t i = 0; i < input.size(); ++i)
    {
        if (input[i] == '!')
            i++;
        else
            str_no_exclamation.push_back(input[i]);
    }
    // parse garbage
    std::string str_no_garbage;
    uint64_t garbage_count = 0;
    for (size_t i = 0; i < str_no_exclamation.size(); ++i)
    {
        if (str_no_exclamation[i] == '<')
        {
            while (str_no_exclamation[++i] != '>')
            {
                garbage_count++;
            }
        }
        else
            str_no_garbage.push_back(str_no_exclamation[i]);
    }
    // parse groups
    uint64_t total_score = 0;
    uint64_t current_group_score = 0;
    for (size_t i = 0; i < str_no_garbage.size(); ++i)
    {
        if (str_no_garbage[i] == '{')
        {
            current_group_score++;
            total_score += current_group_score;
        }
        else if (str_no_garbage[i] == '}')
        {
            current_group_score--;
        }
    }

    outf("Group score {}", total_score);
    outf("Garbage count {}", garbage_count);
}

int main()
{
    input_t input = read();

    solve_1(input);

    return 0;
}
