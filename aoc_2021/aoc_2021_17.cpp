#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include "outf.h"

struct Trench
{
    int32_t x1;
    int32_t x2;
    int32_t y1;
    int32_t y2;
};
using Input = Trench;

Input read_input()
{
    Input input;
    std::string line;
    std::getline(std::cin, line);
    assert(line.size());
    {
        auto parts = split_string(line, ": x=");
        assert(parts.size() == 2);
        line = parts[1];
    }
    {
        auto parts = split_string(line, ", y=");
        assert(parts.size() == 2);
        {
            auto interval = split_string(parts[0], "..");
            assert(interval.size() == 2);
            input.x1 = std::stol(interval[0]);
            input.x2 = std::stol(interval[1]);
        }
        {
            auto interval = split_string(parts[1], "..");
            assert(interval.size() == 2);
            input.y1 = std::stol(interval[0]);
            input.y2 = std::stol(interval[1]);
        }
    }

    return input;
}

void solve_1_2(const Input& input)
{
    
    auto [trench_x1, trench_x2, trench_y1, trench_y2] = input;

    if (std::abs(trench_x2) < std::abs(trench_x1))
        std::swap(trench_x1, trench_x2);
    if (std::abs(trench_y2) < std::abs(trench_y1))
        std::swap(trench_y1, trench_y2);
    
    int32_t max_y_reached = -1000;
    // int32_t max_y_vx = 0;
    // int32_t max_y_vy = 0;
    const int32_t min_trench_x = trench_x1 < trench_x2 ? trench_x1 : trench_x2;
    const int32_t max_trech_x = trench_x1 > trench_x2 ? trench_x1 : trench_x2;
    const int32_t min_trench_y = trench_y1 < trench_y2 ? trench_y1 : trench_y2;
    const int32_t max_trench_y = trench_y1 > trench_y2 ? trench_y1 : trench_y2;
    const int32_t initial_max_probe_y = -1000;
    const int32_t max_steps = 1000;

    int32_t probes_on_target_count = 0;
    for (int32_t initial_vx = -std::abs(trench_x2); initial_vx <= std::abs(trench_x2); initial_vx++)
    {
        for (int32_t initial_vy = std::abs(trench_y2); initial_vy >= -std::abs(trench_y2); initial_vy--)
        {
            if (initial_vx == 6 && initial_vy == 9)
            {
                int32_t a = 1;
            }
            int32_t probe_vx = initial_vx;
            int32_t probe_vy = initial_vy;
            int32_t probe_x = 0;
            int32_t probe_y = 0;
            int32_t step = 0;
            int32_t max_probe_y = initial_max_probe_y;
            
            while (!(probe_x >= min_trench_x && probe_x <= max_trech_x && probe_y >= min_trench_y && probe_y <= max_trench_y) && step++ < max_steps)
            {
                probe_x += probe_vx;
                probe_y += probe_vy;
                probe_vx = (probe_vx > 0) ? probe_vx - 1 : probe_vx++;
                probe_vy--;
                if (probe_y > max_probe_y)
                    max_probe_y = probe_y;
            }
            if (step < max_steps)
            {
                probes_on_target_count++;
                if (max_probe_y > max_y_reached)
                {
                    max_y_reached = max_probe_y;
                    // max_y_vy = initial_vy;
                    // max_y_vx = initial_vx;
                }
                //outf("{}, {} {}\n", vx, vy, maxpy);
            }
        }
    }
    outf("Part 1: {}\n", max_y_reached);
    outf("Part 2: {}\n", probes_on_target_count);
}

int main()
{
    auto input = read_input();
    solve_1_2(input);

    return 0;
}
