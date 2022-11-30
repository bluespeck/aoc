#include <iostream>
#include <string>
#include <vector>
#include "../outf.h"

using Input = std::vector<std::string>;

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

std::pair<uint32_t, uint32_t> count_nth_bit(const Input& input, size_t n)
{
    uint32_t count_0 = 0;
    uint32_t count_1 = 0;
    for (size_t i = 0; i < input.size(); i++)
    {
        if (input[i][n] == '0')
            ++count_0;
        else
            ++count_1;
    }

    return { count_0, count_1 };
}

void solve_1(const Input &input)
{
    outf("\nPart {}: ", 1);

    uint64_t gamma_rate = 0;
    uint64_t epsilon_rate = 0;
    for (size_t i = 0; i < input[0].size(); i++)
    {
        auto [count_0, count_1] = count_nth_bit(input, i);
        if (count_0 > count_1)
        {
            gamma_rate <<= 1;
            epsilon_rate = 1 + (epsilon_rate << 1);
        }
        else
        {
            gamma_rate = 1 + (gamma_rate << 1);
            epsilon_rate <<= 1;
        }
    }

    outf("{}", gamma_rate * epsilon_rate);
}

void remove_if_nth_bit(Input& values, size_t n, char bit_val)
{
    for (size_t j = 0; j < values.size(); )
    {
        if (values[j][n] == bit_val)
        {
            values[j] = values.back();
            values.pop_back();
        }
        else
            j++;
    }
}

uint64_t get_filtered_value(Input values,  bool is_o2_rating)
{
    uint64_t filtered_value = 0;
    for (size_t i = 0; i < values[0].size(); i++)
    {
        auto [count_0, count_1] = count_nth_bit(values, i);
        
        if (count_0 > count_1)
        {
            remove_if_nth_bit(values, i, is_o2_rating ? '1' : '0');
        }
        else
        {
            remove_if_nth_bit(values, i, is_o2_rating ? '0' : '1');
        }
    
        if (values.size() == 1)
        {
            filtered_value = std::stoll(values[0], nullptr, 2);
            break;
        }
    }

    return filtered_value;
}

void solve_2(const Input &input)
{
    outf("\nPart {}: ", 2);
    
    uint64_t o2_rating = get_filtered_value(input, true);
    uint64_t co2_scrubber = get_filtered_value(input, false);

    outf("{}", o2_rating * co2_scrubber);
}

int main()
{
    auto input = read_input();
    solve_1(input);
    solve_2(input);

    return 0;
}
