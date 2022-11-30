#include <cstdint>
#include "x64/../../outf.h"
#include <ranges>
#include <vector>
#include <iostream>
#include <cassert>
#include <unordered_map>

using input_t = struct { uint64_t a, b; };

input_t read()
{
    //input_t input = { 65, 8921 };
    input_t input = { 634, 301 };
    return input;
}

void solve_1(const input_t& input)
{
    auto [a, b] = input;

    int32_t count = 0;

    for (int32_t i = 0; i < 40'000'000; i++)
    {
        a = (a * 16'807) % 2'147'483'647;
        b = (b * 48'271) % 2'147'483'647;

        const int64_t mask = ((1 << 16) - 1);
        if ((a & mask) == (b & mask))
            count++;
    }

    outf("Total count is {}\n", count);
}
void solve_2(const input_t& input)
{
    auto [a, b] = input;

    int32_t count = 0;

    auto find_next_a = [&a]() {
        do
        {
            a = (a * 16'807) % 2'147'483'647;
        } while (a % 4);
        return a;
    };
    auto find_next_b = [&b]() {
        do
        {
            b = (b * 48'271) % 2'147'483'647;
        } while (b % 8);
    };

    for (int32_t i = 0; i < 5'000'000; i++)
    {
        find_next_a();
        find_next_b();

        const int64_t mask = ((1 << 16) - 1);
        if ((a & mask) == (b & mask))
            count++;
    }

    outf("Total count is {}\n", count);
}

int main()
{
    input_t input = read();

    solve_1(input);
    solve_2(input);

    return 0;
}
