#include <cstdint>
#include "x64/../../outf.h"
#include <ranges>
#include <vector>
#include <iostream>
#include <cassert>
#include <unordered_map>
#include <sstream>

struct scanner_t
{
    int32_t depth;
    int32_t range;
    int32_t current_pos = 0;
    int32_t increment = 1;
};

using input_t = std::vector<scanner_t>;

input_t read()
{
    input_t input;

    while (std::cin)
    {
        std::string(line);
        std::getline(std::cin, line);
        if (!line.empty())
        {
            scanner_t scanner;
            std::istringstream ss(line);
            ss >> scanner.depth;
            ss.ignore();
            ss >> scanner.range;
            input.push_back(scanner);
        }
    }
    return input;
}

bool solve_1(const input_t& input, int32_t delay = 0)
{
    input_t states = input;
    int32_t packet_pos = -1;
    int64_t severity = 0;
    int32_t max_range = 0;
    int32_t max_depth = 0;
    bool caught = false;
    for (auto& s : states)
    {
        if (max_range < s.range)
            max_range = s.range;
        if (max_depth < s.depth)
            max_depth = s.depth;
    }
    int32_t pico = 0;

    while (packet_pos <= max_depth)
    {
        //outf("\nPicosecond {}:\n", pico);
        if (delay > 0)
            delay--;
        else
            packet_pos++;
        auto output_stuff = [&]() {
            for (int32_t r = 0; r < max_range; r++)
            {
                if (r == 0)
                {
                    for (int32_t i = 0; i <= max_depth; i++)
                        outf(" {}  ", i);
                    outf("\n");
                }

                for (int32_t i = 0; i <= max_depth; i++)
                {
                    bool found = false;
                    for (auto& s : states)
                        if (s.depth == i && s.range > r)
                        {
                            found = true;
                            if (r == 0 && packet_pos == i)
                                outf("({}) ", s.current_pos == r ? "S" : " ");
                            else
                                outf("[{}] ", s.current_pos == r ? "S" : " ");
                        }
                    if (!found)
                    {
                        if (r == 0 && i == packet_pos)
                            outf("(.) ");
                        else
                            outf("... ");
                    }
                }
                outf("\n");
            }
        };
        //output_stuff();
        // update packet state
        // update scanner states
        for (auto& s : states)
        {
            if (s.current_pos == 0 && packet_pos == s.depth)
            {
                severity += s.depth * s.range;
                caught = true;
            }
            s.current_pos = (s.current_pos + s.increment) % s.range;
            if (s.current_pos == s.range - 1 || s.current_pos == 0)
                s.increment = -s.increment;
        }
        //output_stuff();
        pico++;
    }

    //        outf("Severity = {}.\n", severity);
    
    return caught;
}

void solve_2(const input_t& input)
{
    int32_t delay = 0;
    while (true) {
        bool found = true;
        for (auto& s : input)
        {
            if ((s.depth + delay) % (2 * s.range - 2) == 0)
            {
                found = false;
                break;
            }
        }
        if (found)
            break;
        delay++;
    }
    outf("Delay is {}", delay);
}

int main()
{
    input_t input = read();

    //solve_1(input);
    solve_2(input);

    outf("\nDone.\n");
    std::cin.get();
    return 0;
}