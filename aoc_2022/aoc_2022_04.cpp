#include "../outf.h"

#include <cassert>
#include <string>
#include <vector>

struct interval_pair_t
{
    uint32_t p1, p2;
    uint32_t p3, p4;
};

using input_t = std::vector<interval_pair_t>;
input_t read()
{
    input_t input;

    std::string line;
    while(std::getline(std::cin, line))
    {
        auto pair_parts = split_string(line, ",");
        assert(pair_parts.size() == 2);
        auto pair1 = split_string(pair_parts[0], "-");
        auto pair2 = split_string(pair_parts[1], "-");
        assert(pair1.size() == 2 && pair2.size() == 2);
        input.emplace_back(std::stoull(pair1[0]), std::stoull(pair1[1]), std::stoull(pair2[0]), std::stoull(pair2[1]));
    }

    input_t ic = input;
    return ic;
}


void solve_1(const input_t& input)
{
    int32_t count = 0;
    for (auto e : input)
    {
        if (e.p3 >= e.p1 && e.p4 <= e.p2
            || e.p1 >= e.p3 && e.p2 <= e.p4)
        {
            count++;
        }
    }
    outf("{}\n", count);
}

void solve_2(const input_t& input)
{
    int32_t count = 0;
    for (auto e : input)
    {
        if (e.p3 <= e.p1 && e.p4 >= e.p1
            || e.p3 <= e.p2 && e.p4 >= e.p2
            ||e.p1 <= e.p3 && e.p2 >= e.p3
            || e.p1 <= e.p4 && e.p2 >= e.p4)
        {
            count++;
        }
    }
    outf("{}\n", count);
}

int main()
{
    input_t input = read();
    solve_1(input);
    solve_2(input);

    return 0;
}