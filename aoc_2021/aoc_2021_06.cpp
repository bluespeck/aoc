#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <vector>
#include "outf.h"

using Input = std::vector<int8_t>;

Input read_input()
{
    Input input;
    while (std::cin)
    {
        std::string line;
        std::getline(std::cin, line);
        if (line.size())
        {
            auto strs = split_string(line, ",");
            for (auto e : strs)
                input.push_back(std::stoll(e));
        }
    }
    return input;
}

void print_state(const Input& input, int day)
{
    outf("After {:3} days: ", day);
    for (auto e : input)
        outf("{},", e);
    outf("\b\n");
}

void solve_1(const Input& _input)
{
    Input input = _input;
    int day = 0;
    //print_state(input, day);
    while (day < 80)
    {
        day++;
        for (auto& e : input)
        {
            e = e - 1;
        }
        auto sz = input.size();
        for (int i = 0; i < sz; i++)
        {
            if (input[i] == -1)
            {
                input[i] = 6;
                input.push_back(8);
            }
        }
        //print_state(input, day);
    }
    outf("\nPart {}: ", 1);
    outf("{}", input.size());
}

void solve_2(const Input& input)
{
    outf("\nPart {}: ", 2);

    std::array<int64_t, 9> timers;
    timers.fill(0);
    for (auto e : input)
        timers[e] ++;

    uint32_t day = 0;
    while (day < 256)
    {
        day++;

        int64_t temp = timers[0];
        std::rotate(timers.begin(), timers.begin() + 1, timers.end());
        timers[6] += temp; // recent parent lanternfish
    }
    
    outf("{}", std::accumulate(timers.begin(), timers.end(), 0ll));
}

int main()
{
    auto input = read_input();
    solve_1(input);
    solve_2(input);

    return 0;
}
