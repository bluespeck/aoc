#include <array>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "outf.h"

struct Input {
    std::string polymer;
    std::vector<std::vector<std::string>> rules;
}; 

Input read_input()
{
    Input input;
    std::getline(std::cin, input.polymer);
    while (std::cin)
    {
        std::string line;
        std::getline(std::cin, line);
        if (line.size())
        {
            input.rules.push_back(split_string(line, " -> "));
        }
    }
    return input;
}


void solve_1(const Input& input)
{
    outf("\nPart {}: ", 1);
    std::string polymer = input.polymer;
    for (int step = 0; step < 10; step++)
    {
        std::vector<std::string> pairs;
        for (int i = 0; i < polymer.size() - 1; i++)
        {
            pairs.push_back({ polymer[i], polymer[i + 1] });
        }
        for(auto& pair: pairs)
        for (auto& rule : input.rules)
        {
            if (rule[0][0] == pair[0] && rule[0][1] == pair[1])
            {
                pair = { pair[0], rule[1][0], pair[1] };
                break;
            }

        }
        polymer = "";
        for (auto& pair : pairs)
        {
            polymer += pair[0];
            polymer += pair[1];
        }
        polymer += pairs.back()[2];
    }

    uint32_t counts[26]={};
    for (int i = 0; i < polymer.size(); i++)
    {
        counts[polymer[i] - 'A'] ++;
    }

    uint32_t minc = -1;
    uint32_t maxc = 0;
    for (int i = 0; i < 26; i++)
    {
        if (counts[i] > maxc)
            maxc = counts[i];
        if (counts[i] < minc && counts[i] > 0)
            minc = counts[i];
    }

    outf("{}", maxc - minc);
}

void solve_2(const Input& input)
{
    outf("\nPart {}: ", 2);
    std::string polymer = input.polymer;
    std::unordered_map<std::string, uint64_t> pairs_to_count;
    for (int i = 0; i < polymer.size() - 1; i++)
    {
        std::string pair = { polymer[i], polymer[i + 1] };
           pairs_to_count[pair] ++;
        
    }
    for (int step = 0; step < 40; step++)
    {
        std::unordered_map<std::string, uint64_t> new_pairs_to_count;
        for (auto& [pair, count] : pairs_to_count)
        {
            for (auto& rule : input.rules)
            {
                if (rule[0][0] == pair[0] && rule[0][1] == pair[1])
                {
                    std::string p1 = { pair[0], rule[1][0] };
                    std::string p2 = { rule[1][0], pair[1] };
                    new_pairs_to_count[p1] += count;
                    new_pairs_to_count[p2] += count;
                    break;
                }
            }
        }
        pairs_to_count = new_pairs_to_count;
    }

    uint64_t counts[26] = {};
    for (auto & [pair, count] : pairs_to_count)
    {
        counts[pair[0] - 'A'] += count;
    }
    counts[polymer.back() - 'A']++;

    uint64_t minc = -1;
    uint64_t maxc = 0;
    for (int i = 0; i < 26; i++)
    {
        if (counts[i] > maxc)
            maxc = counts[i];
        if (counts[i] < minc && counts[i] > 0)
            minc = counts[i];
    }

    outf("{}", maxc - minc);
}

int main()
{
    auto input = read_input();
    solve_1(input);
    solve_2(input);

    return 0;
}
