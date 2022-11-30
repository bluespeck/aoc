#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "outf.h"

using Input = std::vector<std::vector<int32_t>>;

Input read_input()
{
    Input input;
    
    while (std::cin)
    {
        std::string line;
        std::getline(std::cin, line);
        if (line.size())
        {
            input.push_back({});
            for (auto ch : line)
                input.back().push_back(ch - '0');
        }
    }
    
    return input;
}

// requires padded matrices (vectors of vectors)
void fill(Input& dists,const Input& input, int32_t i, int32_t j)
{
    static const int32_t di[] = { 1, 0, 0, -1 };
    static const int32_t dj[] = { 0, 1, -1, 0 };
    for (int k = 0; k < 4; k++)
    {
        auto ii = i + di[k];
        auto jj = j + dj[k];
        if (dists[ii][jj] > dists[i][j] + input[ii][jj]
            // don't explore further if distance is already larger than current optimum
            && dists[dists.size() - 2][dists[0].size()-2] > dists[i][j] + input[ii][jj])
        {
            dists[ii][jj] = dists[i][j] + input[ii][jj];
            fill(dists, input, ii, jj);
        }
    }
}

void solve_1(const Input& _input)
{
    auto input = _input;
    Input dists;
    dists.resize(input.size());
    for (auto& dist : dists)
    {
        dist.resize(input[0].size(), std::numeric_limits<int32_t>::max());
    }
    dists[0][0] = 0;
    pad_vec_vec(dists, 0);
    pad_vec_vec(input, 9);
    
    fill(dists, input, 1, 1);

    outf("{}", dists[input.size() - 2][input.size() - 2]);
}

void solve_2(const Input& _input)
{
    Input input;
    input.resize(5 * _input.size());
    for (auto& row : input)
    {
        row.resize(5 * _input[0].size(), 0);
    }

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            for (int ii = 0; ii < _input.size(); ii++)
            {
                for (int jj = 0; jj < _input[ii].size(); jj++)
                {
                    input[i * _input.size() + ii][j * _input[ii].size() + jj] = ((i + j + _input[ii][jj]) > 9  ? (i + j + _input[ii][jj]) - 9 : (i + j + _input[ii][jj]));
                }
            }
        }
    }

    solve_1(input);
}

int main()
{
    auto input = read_input();
    outf("\nPart {}: ", 1);
    solve_1(input);
    outf("\nPart {} -- takes about 15 seconds on my machine with a 100x100 map so bear with it ^^' -- : ", 2);
    solve_2(input);

    return 0;
}
