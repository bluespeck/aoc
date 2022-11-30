#include <cstdint>
#include "x64/../../outf.h"
#include <ranges>
#include <vector>
#include <iostream>
#include <cassert>
#include <unordered_map>

using node_list_t = std::vector<int32_t>;
using input_t = std::unordered_map<int32_t, node_list_t>;

input_t read()
{
    input_t input;

    while (std::cin)
    {
        std::string(line);
        std::getline(std::cin, line);
        if (line.size())
        {
            std::vector<std::string> connection = split_string(line, " <-> ");
            assert(!connection.empty());
            if (connection.size() == 2)
            {
                auto node_id = std::stoi(connection[0]);
                auto strs = split_string(connection[1], ", ");
                for (auto& str : strs)
                {
                    input[node_id].push_back(std::stoi(str));
                }       
            }
        }
    }
    return input;
}

void solve_1(const input_t& input)
{
    node_list_t visited;
    node_list_t to_visit;
    to_visit.push_back(0);
    int32_t current_node;
    while (to_visit.size() > 0)
    {
        current_node = to_visit.back();
        to_visit.pop_back();
        visited.push_back(current_node);
        for (int32_t node : input.at(current_node))
        {
            if (std::ranges::find(visited, node) == visited.end() && std::ranges::find(to_visit, node) == to_visit.end())
            {
                to_visit.push_back(node);
            }
        }
    }

    outf("Total nodes in the same group as 0: {}.\n", visited.size());
}

void solve_2(const input_t& input)
{
    node_list_t visited;
    node_list_t to_visit;
    to_visit.push_back(0);
    int32_t current_node;
    int32_t group_count = 0;
    while (visited.size() < input.size())
    {
        for (auto [key, _] : input)
        {
            if (std::ranges::find(visited, key) == visited.end())
            {
                to_visit.push_back(key);
                group_count++;
                break;
            }
        }
        while (to_visit.size() > 0)
        {
            current_node = to_visit.back();
            to_visit.pop_back();
            visited.push_back(current_node);
            for (int32_t node : input.at(current_node))
            {
                if (std::ranges::find(visited, node) == visited.end() && std::ranges::find(to_visit, node) == to_visit.end())
                {
                    to_visit.push_back(node);
                }
            }
        }
    }
    group_count++;
    outf("Number of groups {}.", group_count);
}
int main()
{
    input_t input = read();

    solve_1(input);
    solve_2(input);
    

    return 0;
}