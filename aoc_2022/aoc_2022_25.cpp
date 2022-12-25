#include "../outf.h"

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

using input_t = std::vector<std::string>;
input_t read()
{
    input_t input;
    std::string line;
    while (std::getline(std::cin, line))
    {
        input.push_back(line);
    }
    return input;
}

std::string convert_base10_to_snafu(int64_t number_base10)
{
    std::string snafu_number;
    while (number_base10 != 0)
    {
        int digit = number_base10 % 5; 
        if (digit == 3)
        {
            snafu_number += '=';
            number_base10 += 2;

        }
        else if (digit == 4)
        {
            snafu_number += '-';
            number_base10 += 1;
        }
        else
        {
            snafu_number += digit + '0';
        }
        number_base10 /= 5;
    }

    std::reverse(snafu_number.begin(), snafu_number.end());
    return snafu_number;
}

int64_t convert_snafu_to_base10(const std::string snafu_number)
{
    int64_t number = 0;
    for (char c : snafu_number)
    {
        number = number * 5;
        switch (c)
        {
        case '=':
            number += -2;
            break;
        case '-':
            number += -1;
            break;
        case '0':
        case '1':
        case '2':
            number += c - '0';
            break;
        }
    }
    return number;
}

void solve_1(const input_t& input)
{
    int64_t sum = 0ull;
    for (auto& e : input)
    {
        auto number = convert_snafu_to_base10(e);
        sum += number;
        auto snafu_number = convert_base10_to_snafu(number);
        assert(e == snafu_number);
    }

    outf("The SNAFU number for Bob is {}\n", convert_base10_to_snafu(sum));
}


int main()
{
    input_t input = read();
    solve_1(input);

    return 0;
}