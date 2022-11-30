#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
#include "outf.h"

using Input = std::vector<int64_t>;

Input read_input()
{
    Input input;
    while (std::cin)
    {
        std::string line;
        std::getline(std::cin, line);
        if (line.size())
        {
            for(auto e: split_string(line, ","))
                input.push_back(std::stoll(e));
        }
    }
    return input;
}

void solve_1(const Input& input)
{
    outf("\nPart {}: ", 1);
    auto [it_min, it_max] = std::minmax_element(input.cbegin(), input.cend());
    int64_t min_pos = *it_min;
    int64_t max_pos = *it_max;
    
    int64_t good_pos = 0;
    int64_t min_sum = std::numeric_limits<int64_t>::max();
    for (int64_t pos = min_pos; pos <= max_pos; pos++)
    {
        int64_t sum = 0;
        for (auto e : input)
        {
            sum += std::abs(e - pos);
        }
        if (min_sum > sum)
        {
            min_sum = sum;
            good_pos = pos;
        }
    }

    outf("{} on pos {}", min_sum, good_pos);
}

void solve_2(const Input& input)
{
    outf("\nPart {}: ", 2);

    auto [it_min, it_max] = std::minmax_element(input.cbegin(), input.cend());
    int64_t min_pos = *it_min;
    int64_t max_pos = *it_max;

    int64_t good_pos = max_pos;
    int64_t min_sum = std::numeric_limits<int64_t>::max();
    for (int64_t pos = min_pos; pos <= max_pos; pos++)
    {
        int64_t sum = 0;
        for (auto e : input)
        {
            int64_t num_steps = std::abs(e - pos);
            int64_t fuel_all_steps = num_steps * (num_steps + 1) / 2;
            sum += fuel_all_steps;
        }
        if (min_sum > sum)
        {
            min_sum = sum;
            good_pos = pos;
        }
    }

    outf("{} on pos {}", min_sum, good_pos);
}

int main()
{
    auto input = read_input();
    solve_1(input);
    solve_2(input);

    return 0;
}
