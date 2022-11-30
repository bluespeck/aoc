#include <cstdint>
#include "x64/../../outf.h"
#include <ranges>
#include <vector>
#include <iostream>
#include <cassert>
#include <unordered_map>

enum class direction_t
{
    n,
    nw,
    sw,
    s,
    se,
    ne
};

using input_t = std::vector<std::string>;

input_t read()
{
    std::string line;
    std::getline(std::cin, line);
    assert(line.size());
    input_t input = split_string(line, ",");

    return input;
}

void solve_1(const input_t& input)
{
    int32_t x = 0, y = 0, p = 0;
    using dir_map_t = std::unordered_map < std::string, int32_t (*)(int32_t) >;
    dir_map_t dir_x, dir_y, dir_p;
    dir_x["n"] = [](int32_t)->int32_t { return 0; };
    dir_y["n"] = [](int32_t)->int32_t { return -1; };
    dir_p["n"] = [](int32_t)->int32_t { return 0; };

    dir_x["nw"] = [](int32_t p)->int32_t { return (p == 0) ? -1 : 0; };
    dir_y["nw"] = [](int32_t p)->int32_t { return (p == 0) ? -1 : 0; };
    dir_p["nw"] = [](int32_t)->int32_t { return 1; };

    dir_x["sw"] = [](int32_t p)->int32_t { return (p == 0) ? -1 : 0; };
    dir_y["sw"] = [](int32_t p)->int32_t { return (p == 0) ? 0: 1; };
    dir_p["sw"] = [](int32_t)->int32_t { return 1; };

    dir_x["s"] = [](int32_t)->int32_t { return 0; };
    dir_y["s"] = [](int32_t)->int32_t { return 1; };
    dir_p["s"] = [](int32_t)->int32_t { return 0; };

    dir_x["se"] = [](int32_t p)->int32_t { return (p == 0) ? 0 : 1; };
    dir_y["se"] = [](int32_t p)->int32_t { return (p == 0) ? 0 : 1; };
    dir_p["se"] = [](int32_t)->int32_t { return 1; };

    dir_x["ne"] = [](int32_t p)->int32_t { return (p == 0) ? 0 : 1; };
    dir_y["ne"] = [](int32_t p)->int32_t { return (p == 0) ? -1 : 0; };
    dir_p["ne"] = [](int32_t)->int32_t { return 1; };
    
    int32_t max_dist = 0;

    for (auto& dir : input)
    {
        x += dir_x[dir](p);
        y += dir_y[dir](p);
        p = (p + dir_p[dir](p)) % 2;
        if (abs(x) + abs(y) > max_dist)
            max_dist = abs(x) + abs(y);
    }

    outf("Coords {},{},{} distance {} max_distance {}\n", x, y, p, abs(x)+abs(y), max_dist);

}

int main()
{
    input_t input = read();

    solve_1(input);

    return 0;
}