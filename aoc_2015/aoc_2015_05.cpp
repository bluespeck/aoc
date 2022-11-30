#include <ranges>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

using input_t = std::vector<std::string>;

input_t
read_input()
{
    input_t input;

    while (std::cin)
    {
        std::string line;
        std::getline(std::cin, line);
        if(line.size())
            input.push_back(line);
    };

    return input;
}

void
solve_1(const input_t& input)
{  
    size_t count = 0;
    for (auto& e : input)
    {
        if (std::ranges::count_if(e, [](char c) {
                return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
            })
            < 3)
        {
            continue;
        }

        bool nice = false;
        for (size_t i = 0; i < e.size() - 1; i++)
        {
            if (e[i] == e[i + 1])
            {
                nice = true;
                break;
            }
        }

        if (nice)
        {
            auto strs = { "ab", "cd", "pq", "xy" };
            
            for (int i = 0; i < e.size() - 1 && nice; i++)
            {
                for (auto& s : strs)
                {
                    if (s[0] == e[i] && s[1] == e[i + 1])
                    {
                        nice = false;
                        break;
                    }
                }
            }
        }
        if (nice)
            count++;
    }
    std::cout << count << std::endl;
}

void
solve_2(const input_t& input)
{
    size_t count = 0;
    for (auto& e : input)
    {
        bool found_pair = false;
        for (size_t i = 0; i < e.size() - 3; i++)
        {
            for (size_t j = i + 2; j < e.size() - 1; j++)
            {
                if (e[i] == e[j] && e[i + 1] == e[j+1])
                {
                    found_pair = true;
                    break;
                }
            }
        }
        if (!found_pair)
            continue;
        bool found_skip_letter = false;
        for (size_t i = 0; i < e.size() - 2; i++)
        {
            if (e[i] == e[i + 2])
            {
                found_skip_letter = true;
                break;
            }
        }
        if (!found_skip_letter)
            continue;

        count++;
    }
    std::cout << count << std::endl;
}

int
main()
{
    input_t input = read_input();

    solve_1(input);
    solve_2(input);

    return 0;
}