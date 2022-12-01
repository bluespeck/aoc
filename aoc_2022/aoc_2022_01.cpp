#include "../outf.h"

#include <algorithm>
#include <numeric>
#include <string>
#include <vector>

using input_t = std::vector<std::vector<int64_t>>;
[[nodiscard]] input_t read()
{
    input_t input;
    std::string line;
    std::vector<int64_t> current_list;
    while (std::getline(std::cin, line))
    {
        if (line.size())
        {
            current_list.push_back(std::stoll(line));
        }
        else
        {
            input.push_back(current_list);
            current_list.clear();
        }
    }
    
    return input;
}


void solve_1(const input_t& input)
{
    int64_t max_calories = 0;
    for (auto& vec : input)
    {
        int64_t total_calories = std::accumulate(vec.begin(), vec.end(), 0ll);
        if (max_calories < total_calories)
        {
            max_calories = total_calories;
        }
    }

    outf("The most total calories an elf has is {}.\n", max_calories);
}

void solve_2(const input_t& input)
{
    std::vector<int64_t> calories;
    for (auto& vec : input)
    {
        calories.push_back(std::accumulate(vec.begin(), vec.end(), 0ll));
    }
    std::sort(calories.begin(), calories.end(), std::greater<>());
    
    outf("The sum of the top three calories holders is {}.\n", calories[0] + calories[1] + calories[2]);
}

int main()
{
    input_t input = read();
    solve_1(input);
    solve_2(input);

    return 0;
}