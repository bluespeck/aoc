#include <array>
#include <iostream>
#include <string>
#include <vector>
#include "outf.h"

// A grid of unsigned char works because the total number of flashes does not
// exceed 100 per step (since it's a 10x10 grid). That is, 100 + 57 (ascii for '9') 
// does not exceed 255; we bring every octopus back to '0' after it flashes.
using ustring = std::basic_string<unsigned char>;
using Input = std::vector<ustring>;

Input read_input()
{
    Input input;
    while (std::cin)
    {
        std::string line;
        std::getline(std::cin, line);
        if (line.size())
        {
            ustring uline{ line.begin(), line.end() };
            input.push_back(uline);
        }
    }
    return input;
}



void solve_1_2(const Input& _input)
{
    Input input = _input;
    size_t total_flash_count = 0;
    for (int32_t step = 1; step < 10000; step++)
    {
        for (int32_t i = 0; i < input.size(); i++)
        {
            for (int32_t j = 0; j < input[i].size(); j++)
            {
                input[i][j]++;
            }
        }
        
        bool can_flash = true;
        int flashes_this_step = 0;
        
        std::array<std::array<char, 10>, 10> already_flashed;
        for (auto& row : already_flashed)
        {
            row.fill('0');
        }
        while (can_flash)
        {
            can_flash = false;
            for (int32_t i = 0; i < input.size() && !can_flash; i++)
            {
                for (int32_t j = 0; j < input[i].size() && !can_flash; j++)
                {
                    if (input[i][j] > '9' && already_flashed[i][j] == '0')
                    {
                        already_flashed[i][j] = '1';
                        can_flash = true;
                        flashes_this_step++;
                        total_flash_count++;
                        for (int di = -1; di <= 1; di++)
                        {
                            for (int dj = -1; dj <= 1; dj++)
                            {
                                auto ii = i + di;
                                auto jj = j + dj;
                                if (ii >= 0 && jj >= 0 
                                    && ii < input.size() && jj < input[ii].size())
                                {
                                    input[ii][jj]++;
                                }
                            }
                        }
                    }
                }
            }
        }
        for (int32_t i = 0; i < input.size(); i++)
        {
            for (int32_t j = 0; j < input[i].size(); j++)
            {
                if (input[i][j] > '9')
                {
                    input[i][j] = '0';
                }
            }
        }
        
        if (step == 100)
        {
            outf("\nPart 1: {}", total_flash_count);
        }
        if (flashes_this_step == 100)
        {
            outf("\nPart 2: {}", step);
            if(step >= 100)
                break;
        }
    }
}

int main()
{
    auto input = read_input();
    solve_1_2(input);

    return 0;
}
