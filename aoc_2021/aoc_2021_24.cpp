#include <cassert>
#include <array>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>
#include "outf.h"

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

struct ALU
{
    std::vector<int64_t> input_buffer;
    int64_t w = 0;
    int64_t x = 0;
    int64_t y = 0;
    int64_t z = 0;
};

void inp(ALU& alu, char a)
{
    assert(!alu.input_buffer.empty());
    *(&alu.w + (a - 'w')) = alu.input_buffer.back();
    alu.input_buffer.pop_back();
}

void add(ALU& alu, char a, std::string b)
{
    auto& a_var = *(&alu.w + (a - 'w'));
    if (b[0] >= '0' && b[0] <= '9' || b[0] == '-')
    {
        a_var = a_var + std::stol(b);
    }
    else
    {
        a_var = a_var + *(&alu.w + (b[0] - 'w'));
    }
}

void mul(ALU& alu, char a, std::string b)
{
    auto& a_var = *(&alu.w + (a - 'w'));
    if (b[0] >= '0' && b[0] <= '9' || b[0] == '-')
    {
        a_var = a_var * std::stol(b);
    }
    else
    {
        a_var = a_var * *(&alu.w + (b[0] - 'w'));
    }
}

void div(ALU& alu, char a, std::string b)
{
    auto& a_var = *(&alu.w + (a - 'w'));
    if (b[0] >= '0' && b[0] <= '9' || b[0] == '-')
    {
        a_var = a_var / std::stol(b);
    }
    else
    {
        a_var = a_var / *(&alu.w + (b[0] - 'w'));
    }
}

void mod(ALU& alu, char a, std::string b)
{
    auto& a_var = *(&alu.w + (a - 'w'));
    if (b[0] >= '0' && b[0] <= '9' || b[0] == '-')
    {
        a_var = a_var % std::stol(b);
    }
    else
    {
        a_var = a_var % *(&alu.w + (b[0] - 'w'));
    }
}

void eql(ALU& alu, char a, std::string b)
{
    auto& a_var = *(&alu.w + (a - 'w'));
    if (b[0] >= '0' && b[0] <= '9' || b[0] == '-')
    {
        a_var = a_var == std::stol(b) ? 1 : 0;
    }
    else
    {
        a_var = a_var == *(&alu.w + (b[0] - 'w')) ? 1 : 0;
    }
}

void run_program(const Input& input, ALU& alu)
{
    for (auto e : input)
    {
        switch (e[0])
        {
        case 'i': inp(alu, e[4]); break;
        case 'a': add(alu, e[4], e.substr(6)); break;
        case 'm': (e[1] == 'u')? mul(alu, e[4], e.substr(6)) : mod(alu, e[4], e.substr(6)); break;
        case 'd': div(alu, e[4], e.substr(6)); break;
        case 'e': eql(alu, e[4], e.substr(6)); break;
        default: assert(false);
        }
    }
}

void solve_1(const Input& input)
{
    outf("\nPart {}: ", 1);


    ALU alu;
    alu.input_buffer.reserve(16);
    // 2'345'842'549
    //for (int64_t i = 99'999'999'999'999; i >= 11'111'111'111'111 ; i--)
    //for (int64_t i = 11'111'111'111'111; i <= 99'999'999'999'999; i++)
    //{   
    //    alu.input_buffer.clear();
    //    int64_t n = i;
    //  //  int64_t n = 13579246899999;
    //    while (n != 0)
    //    {
    //        int64_t val = n % 10;
    //        if (val == 0)
    //            break;
    //        alu.input_buffer.push_back(val);
    //        n /= 10;
    //    }
    //    if (n > 0)
    //        continue;
    //    alu.x = alu.y = alu.z = alu.w = 0;
    //    
    //    run_program(input, alu);
    //    //outf("{} => {} {} {} {}\n", i, alu.w, alu.x, alu.y, alu.z);
    //    if (alu.z == 0)
    //    {
    //        outf("{}", i);
    //        break;
    //    }
    //}

    int64_t div_values[] = { 1,   1,  1,  26, 1,  1,  26,  26, 26,  1,  1,  26, 26, 26 };
    int64_t add_x_values[] = { 11,  13, 15, -8, 13, 15, -11, -4, -15, 14, 14, -1, -8, -14 };
    int64_t add_y_values[] = { 6,   14, 14, 10, 9,  12, 8,   13, 12,  6,  9,  15, 4,  10 };

    // d0 <= 9;
    // d1 <= 9;
    // d2 <= d3 - (14-8) = d3 - 6 => 1<= d2 <= 3 
    // d3 <= d8 - (10 -15) = d8 +5 => 4 <= d8 <= 9 => 1<= d3 <= 9
    // d4 <= d7 - (9-4) = d7 -5 => 1 <= d4 <= 4 => 6 <= d7
    // d5 <= d6 - (12-11) = d6 - 1 => 1 <= d5 <= 8 => 2<=d6
    // d6 <= 9
    // d7 <= 9
    // d8 <= d13 - (12-14) = d13 + 2 => 1<= d13 <= 9 d13 ; 
    // d9 <= d12 - (6-8) = d12 - 2
    // d10 <= d11 - (9-1) = d11 - 8

    int32_t d10 = 1;
    int32_t d11 = 9;
    int32_t count = 0;
    //99'394'899'471'997
    for (int32_t d0 : {1, 2, 3, 4, 5, 6, 7, 8, 9})
        for (int32_t d1 : {1, 2, 3, 4, 5, 6, 7, 8, 9})
            for (int32_t d2 : {1, 2, 3})
                for (int32_t d3 : {1, 2, 3, 4, 5, 6, 7, 8, 9})
                    for (int32_t d4 : {1, 2, 3, 4})
                        for (int32_t d5 : {1, 2, 3, 4, 5, 6, 7, 8})
                            for (int32_t d6 : {1, 2, 3, 4, 5, 6, 7, 8, 9})
                                for (int32_t d7 : {1, 2, 3, 4, 5, 6, 7, 8, 9})
                                    for (int32_t d8 : {1, 2, 3, 4, 5, 6, 7, 8, 9})
                                        for (int32_t d9 : {1, 2, 3, 4, 5, 6, 7})
                            for(int32_t d12:{1,2, 3,4,5,6,7,8,9})
                                for (int32_t d13 : {1,2, 3, 4, 5, 6, 7, 8, 9})
                                {
                                    int32_t digits[14] = { d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13 };
                                    int64_t x = 0;
                                    int64_t z = 0;
                                    for (size_t d = 0; d < 14; d++)
                                    {
                                        auto digit = digits[d];
                                        auto add_x = add_x_values[d];
                                        auto div_z = div_values[d];
                                        auto add_y = add_y_values[d];
                                        if (add_x > 9 && div_z == 1)
                                        {
                                            z = 26 * z + digit + add_y;
                                        }
                                        else
                                        {
                                            x = (z % 26 + add_x) != digit;
                                            z = (25 * x + 1) * (z / div_z) + x * (digit + add_y);
                                        }
                                        //outf("{} ", digit + add_y);
                                    }
                                    //outf("\n");
                                    if (z == 0)
                                    {
                                        for(auto& digit: digits)
                                            outf("{}", digit);
                                        outf("---\n");
                                        return;
                                    }
                                    count++;
                                }
    outf("\nCount {}", count);
    //               01 234 567 890 123
    //for (int64_t i = 99'394'899'471'997; i <= 99'999'999'999'999; i++)
    //for (int64_t i = 11'111'111'111'111; i <= 99'999'999'999'999; i++)
    //{
    //    std::vector<int8_t> digits;
    //    auto n = i;
    //    while (n != 0)
    //    {
    //        int8_t val = static_cast<int8_t>(n % 10);
    //        if (val == 0)
    //            break;
    //        digits.push_back(val);
    //        n /= 10;
    //    }
    //    if (n > 0)
    //        continue;
    //    std::reverse(digits.begin(), digits.end());
    //    int64_t x = 0;
    //    int64_t z = 0;
    //    //outf("z = ");
    //    for (size_t d = 0; d < 14; d++)
    //    {
    //        auto digit = digits[d];
    //        auto add_x = add_x_values[d];
    //        auto div_z = div_values[d];
    //        auto add_y = add_y_values[d];
    //        if (add_x > 9 && div_z == 1)
    //        {
    //            z = 26 * z + digit + add_y;
    //        }
    //        else
    //        {
    //            x = (z % 26 + add_x) != digit;
    //            z = (25 * x + 1) * (z / div_z) + x * (digit + add_y);
    //        }
    //        //outf("{} ", digit + add_y);
    //    }
    //    //outf("\n");
    //    if (z == 0)
    //    {
    //        outf("{}\n", i);
    //    }
    //}

}

void solve_2(const Input& input)
{
    outf("\nPart {}: ", 2);

}

int main()
{
    auto input = read_input();
    solve_1(input);
    solve_2(input);

    return 0;
}
