#include <algorithm>
#include <iostream>
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

void solve_1(const Input& input)
{
    outf("\nPart {}: ", 1);
    int32_t sum = 0;
    for (int i = 0; i < input.size(); i++)
    {
        for (int j = 0; j < input[i].size(); j++)
        {
            int di[4] = { -1, 1, 0, 0 };
            int dj[4] = { 0, 0, -1, 1 };
            bool is_min = true;
            for(int k = 0; k < 4; k++)
            { 
                if (i + di[k] >= 0
                    && i + di[k] < input.size()
                    && j + dj[k] >= 0
                    && j + dj[k] < input[i].size()
                    && input[i][j] >= input[i + di[k]][j + dj[k]])
                {
                    is_min = false;
                    break;
                }
            }
            if (is_min)
                sum += input[i][j] - '0' + 1;
        }
    }
    outf("{}", sum);
}

void fill(Input& input, int ii, int jj, int& basin_size)
{
    if (ii < 0 || jj < 0 || ii >= input.size() || jj >= input[ii].size() || input[ii][jj] == '9')
        return;
    input[ii][jj] = '9';
    basin_size++;
    int di[4] = { -1, 1, 0, 0 };
    int dj[4] = { 0, 0, -1, 1 };
    for (int k = 0; k < 4; k++)
    {
        fill(input, ii + di[k], jj + dj[k], basin_size);
    }
}

void solve_2(const Input& _input)
{
    outf("\nPart {}: ", 2);
    auto input = _input;
    std::vector<int32_t> basin_sizes;
    for (int i = 0; i < input.size(); i++)
    {
        for (int j = 0; j < input.size(); j++)
        {
            if (input[i][j] != '9')
            {
                int32_t basin_size = 0;
                fill(input, i, j, basin_size);
                basin_sizes.push_back(basin_size);
            }
        }
    }

    std::sort(basin_sizes.begin(), basin_sizes.end(), std::greater());

    outf("{}", basin_sizes[0] * basin_sizes[1] * basin_sizes[2]);
}

int main()
{
    auto input = read_input();
    solve_1(input);
    solve_2(input);

    return 0;
}
