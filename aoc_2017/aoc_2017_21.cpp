#include "../outf.h"

#include <cassert>
#include <iostream>
#include <format>
#include <string>
#include <vector>


using pattern_t = std::vector<std::string>;

void print_pattern(const pattern_t& pattern)
{
    for (size_t i = 0; i < pattern.size(); i++)
    {
        outf("{}\n", pattern[i]);
    }

    outf("\n");
}

void print_patterns_side_by_side(const pattern_t& p1, const pattern_t& p2)
{
    for (size_t i = 0; i < p1.size(); i++)
    {
        outf("{}{}{}\n", p1[i], (i == 0 ? " -> " : "    "), p2[i]);
    }
    if (p1.size() < p2.size())
    {
        std::string filler(p1.size(), ' ');
        outf("{}{}{}\n", filler, "    ", p2.back());
    }
    outf("\n");
}

pattern_t rotate_left(const pattern_t& pattern)
{
    pattern_t out = pattern;
    if (size(pattern) == 2)
    {
        out[0][0] = pattern[0][1];
        out[0][1] = pattern[1][1];
        out[1][0] = pattern[0][0];
        out[1][1] = pattern[1][0];
    }
    else // size 3
    {
        out[0][0] = pattern[0][2];
        out[0][1] = pattern[1][2];
        out[0][2] = pattern[2][2];
        out[1][0] = pattern[0][1];
        out[1][2] = pattern[2][1];
        out[2][0] = pattern[0][0];
        out[2][1] = pattern[1][0];
        out[2][2] = pattern[2][0];
    }
    return out;
}

pattern_t flip_h(const pattern_t& pattern)
{
    pattern_t out = pattern;
    if (size(pattern) == 2)
    {
        out[0][0] = pattern[0][1];
        out[1][0] = pattern[1][1];
        out[0][1] = pattern[0][0];
        out[1][1] = pattern[1][0];
    }
    else // size 3
    {
        out[0][0] = pattern[0][2];
        out[1][0] = pattern[1][2];
        out[2][0] = pattern[2][2];
        out[0][2] = pattern[0][0];
        out[1][2] = pattern[1][0];
        out[2][2] = pattern[2][0];
    }
    return out;
}

pattern_t flip_v(const pattern_t& pattern)
{
    pattern_t out = pattern;
    if (pattern.size() == 2)
    {
        out[0][0] = pattern[1][0];
        out[1][0] = pattern[0][0];
        out[0][1] = pattern[1][1];
        out[1][1] = pattern[0][1];
    }
    else // size 3
    {
        out[0][0] = pattern[2][0];
        out[0][1] = pattern[2][1];
        out[0][2] = pattern[2][2];
        out[2][0] = pattern[0][0];
        out[2][1] = pattern[0][1];
        out[2][2] = pattern[0][2];
    }
    return out;
}


std::vector<pattern_t> generate_variations(const pattern_t& pattern)
{
    //outf("--------------------- beginning generating variations\n");
    std::vector<pattern_t> variations;
    variations.push_back(pattern);
    for (int i = 0; i < 3; i++)
    {
        variations.push_back(rotate_left(variations.back()));
        //outf("rotate_left[{}]\n", i + 1);
        //print_patterns_side_by_side(variations[i], variations.back());
    }
    for (int i = 0; i < 2; i++)
    {
        variations.push_back(flip_h(variations[i]));
        //outf("flip_h\n");
        //print_patterns_side_by_side(variations[i], variations.back());
        variations.push_back(flip_v(variations[i]));
        //outf("flip_v\n");
        //print_patterns_side_by_side(variations[i], variations.back());
    }
    //outf("--------------------- ending generating variations\n");
    return variations;
}

struct rule_t
{
    pattern_t input;
    pattern_t output;
};

using rule_book_t = std::vector<rule_t>;
using input_t = rule_book_t;
input_t read()
{
    input_t input;

    std::string line;
    while (std::getline(std::cin, line))
    {
        rule_t rule;
        auto rule_parts = split_string(line, " => ");
        assert(rule_parts.size() == 2);
        rule.input = split_string(rule_parts[0], "/");
        rule.output = split_string(rule_parts[1], "/");
        input.push_back(rule);
    }
    return input;
}

std::vector<pattern_t> split_grid_into_patterns(const std::vector<std::string>& grid)
{
    size_t pattern_size = 0;
    size_t patterns_per_side = 0;
    std::string empty_pattern_row;
    if (grid.size() % 2 == 0)
    {
        pattern_size = 2;
        patterns_per_side = grid.size() / 2;
        empty_pattern_row = "..";
    }
    else
    {
        assert(grid.size() % 3 == 0);
        pattern_size = 3;
        patterns_per_side = grid.size() / 3;
        empty_pattern_row = "...";
    }

    std::vector<pattern_t> sub_patterns;
    sub_patterns.resize(patterns_per_side * patterns_per_side);
    for (size_t i = 0; i < patterns_per_side; i++)
    {
        for (size_t j = 0; j < patterns_per_side; j++)
        {
            auto& pattern = sub_patterns[i * patterns_per_side + j];
            for (size_t k = 0; k < pattern_size; k++)
            {
                pattern.push_back(empty_pattern_row);
                for (size_t l = 0; l < pattern_size; l++)
                {
                    pattern.back()[l] = grid[i * pattern_size + k][j * pattern_size + l];
                }
            }
        }
    }

    return sub_patterns;
}
#pragma optimize("",off)
std::vector<std::string> merge_patterns(const std::vector<pattern_t>& patterns)
{
    size_t pattern_size = 0;
    size_t patterns_per_side = 0;

    pattern_size = patterns[0].size();
    patterns_per_side = sqrt(patterns.size());

    std::vector<std::string> grid;
    grid.resize(patterns_per_side * pattern_size);
    for (size_t i = 0; i < patterns_per_side; i++)
    {
        for (size_t j = 0; j < patterns_per_side; j++)
        {
            for (size_t k = 0; k < pattern_size; k++)
            {
                //outf("Trying to add {}\n", patterns[i * patterns[0].size() + j][k]);
                grid[i * pattern_size + k] += patterns[i * patterns_per_side + j][k];
            }
        }
    }

    return grid;
}
#pragma optimize("",on)
bool are_patterns_equal(const pattern_t& p1, const pattern_t& p2)
{
    if (p1.size() != p2.size())
        return false;
    
    for(size_t i = 0; i < p1.size(); i++)
    {
        if (p1[i] != p2[i])
            return false;
    }

    return true;
}

pattern_t transform_pattern(const pattern_t& pattern, const rule_book_t& rule_book)
{
    auto pattern_variations = generate_variations(pattern);
    for (auto& pattern_variation : pattern_variations)
    {
        for (auto& rule : rule_book)
        {
            if (are_patterns_equal(rule.input, pattern_variation))
            {
                return rule.output;
            }
        }
    }
    assert(false && "Could not transform, no rule!");
}



void solve_1(const input_t& input)
{
    std::vector<std::string> grid;
    grid.push_back(".#.");
    grid.push_back("..#");
    grid.push_back("###");

    size_t current_iteration = 0;

    while (current_iteration++ < 5)
    {
        outf("Step {}. Starting with \n", current_iteration);
        print_pattern(grid);
        outf("After split:\n");
        auto patterns = split_grid_into_patterns(grid);
        for (auto& pattern : patterns)
        {
            auto old_pattern = pattern;
            pattern = transform_pattern(pattern, input);
            print_patterns_side_by_side(old_pattern, pattern);
        }
        grid = merge_patterns(patterns);
        outf("after merge:\n");
        print_pattern(grid);
    }

    size_t count = 0;
    for (auto& e : grid)
    {
        count += std::count(e.begin(), e.end(), '#');
    }
    outf("Lit pixels: {}\n", count);
}

void solve_2(const input_t& input)
{
    std::vector<std::string> grid;
    grid.push_back(".#.");
    grid.push_back("..#");
    grid.push_back("###");

    size_t current_iteration = 0;

    while (current_iteration++ < 18)
    {
        auto patterns = split_grid_into_patterns(grid);
        for (auto& pattern : patterns)
        {
            auto old_pattern = pattern;
            pattern = transform_pattern(pattern, input);
        }
        grid = merge_patterns(patterns);
    }

    //print_pattern(grid);

    size_t count = 0;
    for (auto& e : grid)
    {
        count += std::count(e.begin(), e.end(), '#');
    }
    outf("Lit pixels: {}\n", count);
}

int main()
{
    input_t input = read();
    solve_1(input);
    solve_2(input);

    return 0;
}