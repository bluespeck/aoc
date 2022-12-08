#include "../outf.h"

#include <algorithm>
#include <utility>
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


void solve_1(const input_t& input)
{
    std::vector<std::pair<size_t, size_t>> visible_trees;

    for (size_t i = 1; i <= input.size(); i++)
    {   
        int current_height = -1;
        for (size_t j = 1; j < input[i-1].size(); j++)
        {
            int height = input[i-1][j-1] - '0';
            if (height > current_height)
            {
                current_height = current_height = height;
                if (std::find(visible_trees.cbegin(), visible_trees.cend(), std::pair{ i - 1, j - 1 }) == visible_trees.cend())
                {
                    visible_trees.emplace_back(i - 1, j - 1);
                }
            }
        }
    }

    for (size_t i = 1; i <= input.size(); i++)
    {
        int current_height = -1;
        for (size_t j = input[i - 1].size(); j >= 1; j--)
        {
            int height = input[i - 1][j - 1] - '0';
            if (height > current_height)
            {
                current_height = current_height = height;
                if (std::find(visible_trees.cbegin(), visible_trees.cend(), std::pair{ i - 1, j - 1 }) == visible_trees.cend())
                {
                    visible_trees.emplace_back(i - 1, j - 1);
                }
            }
        }
    }

    for (size_t j = 1; j < input[0].size(); j++)
    {
        int current_height = -1;
        for (size_t i = 1; i <= input.size(); i++)
        {
            int height = input[i - 1][j - 1] - '0';
            if (height > current_height)
            {
                current_height = current_height = height;
                if (std::find(visible_trees.cbegin(), visible_trees.cend(), std::pair{ i - 1, j - 1 }) == visible_trees.cend())
                {
                    visible_trees.emplace_back(i - 1, j - 1);
                }
            }
        }
    }

    for (size_t j = 1; j < input[0].size(); j++)
    {
        int current_height = -1;
        for (size_t i = input.size(); i >= 1; i--)
        {
            int height = input[i - 1][j - 1] - '0';
            if (height > current_height)
            {
                current_height = current_height = height;
                if (std::find(visible_trees.cbegin(), visible_trees.cend(), std::pair{ i - 1, j - 1 }) == visible_trees.cend())
                {
                    visible_trees.emplace_back(i - 1, j - 1);
                }
            }
        }
    }

    outf("{} visible trees.\n", visible_trees.size());
}

size_t scenic_score(const input_t& input, size_t ii, size_t jj)
{
    size_t tree_height = input[ii][jj] - '0';
    size_t view_distance_left = 0;
    size_t view_distance_up = 0;
    size_t view_distance_down = 0;
    size_t view_distance_right = 0;

    for (size_t i = ii; i >= 1; i--)
    {
        view_distance_up++;
        if (tree_height <= input[i - 1][jj] - '0')
        {
            break;
        }
    }

    for (size_t i = ii + 2; i <= input.size(); i++)
    {
        view_distance_down++;
        if (tree_height <= input[i - 1][jj] - '0')
        {
            break;
        }
    }

    for (size_t j = jj; j >= 1; j--)
    {
        view_distance_left++;
        if (tree_height <= input[ii][j - 1] - '0')
        {
            break;
        }
    }

    for (size_t j = jj + 2; j <= input[ii].size(); j++)
    {
        view_distance_right++;
        if (tree_height <= input[ii][j - 1] - '0')
        {
            break;
        }
    }

    return view_distance_left * view_distance_up * view_distance_down * view_distance_right;
}

void solve_2(const input_t& input)
{
    size_t max_score = 0;
    std::pair<size_t, size_t> coords;
    for (size_t i = 0; i < input.size(); i++)
    {
        for (size_t j = 0; j < input[i].size(); j++)
        {
            size_t score = scenic_score(input, i, j);
            if (score > max_score)
            {
                max_score = score;
                coords = { i,j };
            }
        }
    }

    outf("The highest scenic score was {}, for the tree at ({}, {}), with height {}.\n", max_score, coords.first, coords.second, input[coords.first][coords.second]);
}

int main()
{
    input_t input = read();
    solve_1(input);
    solve_2(input);

    return 0;
}