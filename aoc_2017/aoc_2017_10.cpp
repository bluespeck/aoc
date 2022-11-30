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
    std::getline(std::cin, input);

    return input;
}

void solve_1(const input_t& input)
{
    using list_t = std::vector<int64_t>;
    list_t lengths;
    for (auto& str : split_string(input, ","))
    {
        lengths.push_back(std::stoll(str));
    }

    list_t list;
    for (size_t i = 0; i < 256; i++)
        list.push_back(i);

    size_t current_position = 0;
    size_t skip_size = 0;

    for (auto length : lengths)
    {
        for (size_t i = 0; i < length / 2; i++)
        {
            std::swap(list[(current_position + i) % list.size()], list[(current_position + (length - i - 1)) % list.size()]);
        }

        current_position = (current_position + length + skip_size++) % list.size();
    }

    outf("Product is {}\n", list[0] * list[1]);
}

void solve_2(const input_t& input)
{
    using list_t = std::vector<int64_t>;
    list_t lengths;
    for (auto ch : input)
    {
        lengths.push_back(static_cast<int64_t>(ch));
    }
    lengths.push_back(17);
    lengths.push_back(31);
    lengths.push_back(73);
    lengths.push_back(47);
    lengths.push_back(23);

    list_t list;
    for (size_t i = 0; i < 256; i++)
        list.push_back(i);

    size_t current_position = 0;
    size_t skip_size = 0;

    for (int count = 0; count < 64; count++)
    {
        for (auto length : lengths)
        {
            for (size_t i = 0; i < length / 2; i++)
            {
                std::swap(list[(current_position + i) % list.size()], list[(current_position + (length - i - 1)) % list.size()]);
            }

            current_position = (current_position + length + skip_size++) % list.size();
        }
    }

    outf("\nHash is is ");
    for (int i = 0; i < 16; i++)
    {
        int64_t xor16 = 0;
        for (int j = 0; j < 16; j++)
        {
            xor16 ^= list[i * 16 + j];
        }
        outf("{:02x}", xor16);
    }

}

int main()
{
    input_t input = read();

    solve_1(input);
    solve_2(input);

    return 0;
}
