#include <iostream>
#include <vector>
#include "../../outf.h"

using Input = std::vector<std::string>;

struct Stats {
    int64_t x = 0;
    int64_t depth = 0;
    int64_t aim = 0;
};

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

void solve_1(const Input input)
{
    outf("\nPart {}: ", 1);

    Stats stats;
    for (auto& line : input)
    {
        if (line[0] == 'u')
            stats.depth -= std::stoll(line.substr(3));
        else if (line[0] == 'd')
            stats.depth += std::stoll(line.substr(5));
        else if (line[0] == 'f')
        {
            stats.x += std::stoll(line.substr(8));
        }
    }

    outf("{}", stats.x * stats.depth);
}

void solve_2(const Input input)
{
    outf("\nPart {}: ", 2);

    Stats stats;
    for (auto& line : input)
    {
        if (line[0] == 'u')
            stats.aim -= std::stoll(line.substr(3));
        else if (line[0] == 'd')
            stats.aim += std::stoll(line.substr(5));
        else if (line[0] == 'f')
        {
            int64_t val = std::stoll(line.substr(8));
            stats.x += val;
            stats.depth += stats.aim * val;
        }
    }

    outf("{}", stats.x * stats.depth);
}

int main()
{
    auto input = read_input();
    solve_1(input);
    solve_2(input);

    return 0;
}
