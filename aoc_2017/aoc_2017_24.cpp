#include "../outf.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <format>
#include <span>
#include <string>
#include <vector>

struct component_t
{
    int p1, p2;
};

using input_t = std::vector<component_t>;
input_t read()
{
    input_t input;
    std::string line;
    while (std::getline(std::cin, line))
    {
        auto component_pins = split_string(line, "/");
        input.emplace_back(std::stoi(component_pins[0]), std::stoi(component_pins[1]));
    }
    return input;
}

using bridge_t = std::vector<int>;

void backtrace(bridge_t& bridge, const input_t& input, std::vector<bool>& used, int32_t sum, int32_t& max_sum)
{
    for (size_t i = 0; i < used.size(); i++)
    {
        if (!used[i])
        {
            if (input[i].p1 == bridge.back())
            {
                bridge.push_back(input[i].p2);
                sum += input[i].p1 + input[i].p2;
                if (max_sum < sum)
                {
                    max_sum = sum;
                }
                used[i] = true;
                backtrace(bridge, input, used, sum, max_sum);
                used[i] = false;
                bridge.pop_back();
                sum -= input[i].p1 + input[i].p2;
            }
            else if (input[i].p2 == bridge.back())
            {
                bridge.push_back(input[i].p1);
                sum += input[i].p1 + input[i].p2;
                if (max_sum < sum)
                {
                    max_sum = sum;
                }
                used[i] = true;
                backtrace(bridge, input, used, sum, max_sum);
                used[i] = false;
                bridge.pop_back();
                sum -= input[i].p1 + input[i].p2;
            }
        }
    }

    //for (int32_t i : bridge)
    //{
    //    outf("{} ", i);
    //}
    //outf("\n");
}

void backtrace_2(bridge_t& bridge, const input_t& input, std::vector<bool>& used, int32_t sum, size_t& max_length, int32_t& max_sum)
{
    for (size_t i = 0; i < used.size(); i++)
    {
        if (!used[i])
        {
            if (input[i].p1 == bridge.back())
            {
                bridge.push_back(input[i].p2);
                sum += input[i].p1 + input[i].p2;
                if (max_length < bridge.size() || max_length == bridge.size() && max_sum < sum)
                {
                    max_length = bridge.size();
                    max_sum = sum;
                }
                used[i] = true;
                backtrace_2(bridge, input, used, sum, max_length, max_sum);
                used[i] = false;
                bridge.pop_back();
                sum -= input[i].p1 + input[i].p2;
            }
            else if (input[i].p2 == bridge.back())
            {
                bridge.push_back(input[i].p1);
                sum += input[i].p1 + input[i].p2;
                if (max_length < bridge.size() || max_length == bridge.size() && max_sum < sum)
                {
                    max_length = bridge.size();
                    max_sum = sum;
                }
                used[i] = true;
                backtrace_2(bridge, input, used, sum, max_length, max_sum);
                used[i] = false;
                bridge.pop_back();
                sum -= input[i].p1 + input[i].p2;
            }
        }
    }
}

void solve_1(const input_t& input)
{
    bridge_t bridge;
    std::vector<bool> used;
    used.resize(input.size());
    int32_t max_sum = 0;
    bridge.push_back(0);
    backtrace(bridge, input, used, 0, max_sum);
    outf("{}\n", max_sum);
}

void solve_2(const input_t& input)
{
    bridge_t bridge;
    std::vector<bool> used;
    used.resize(input.size());
    int32_t max_sum = 0;
    size_t max_length = 0;
    bridge.push_back(0);
    backtrace_2(bridge, input, used, 0, max_length, max_sum);
    outf("{}\n", max_sum);
}

int main()
{
    input_t input = read();
    solve_1(input);
    solve_2(input);

    return 0;
}