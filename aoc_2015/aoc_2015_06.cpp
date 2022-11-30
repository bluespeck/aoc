#include <ranges>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>

struct point_t
{
    int64_t x, y;
};

struct rectangle_t
{
    point_t top_left;
    point_t bottom_right;
};

enum struct instruction_type
{
    toggle,
    turn_on,
    turn_off
};

struct instruction_t
{
    instruction_type action;
    rectangle_t rectangle;
};

using input_t = std::vector<instruction_t>;

input_t
read_input()
{
    input_t input;

    while (std::cin)
    {
        instruction_t instr;
        std::string action;
        if (std::cin >> action)
        {
            if (action == "toggle")
            {
                instr.action = instruction_type::toggle;
            }
            else if (std::cin >> action)
            {
                if (action == "on")
                {
                    instr.action = instruction_type::turn_on;
                }
                else
                {
                    instr.action = instruction_type::turn_off;
                }
            }
        }
        std::cin >> instr.rectangle.top_left.x;
        std::cin.ignore();
        std::cin >> instr.rectangle.top_left.y;
        std::cin.ignore(9);
        std::cin >> instr.rectangle.bottom_right.x;
        std::cin.ignore();
        if(std::cin >> instr.rectangle.bottom_right.y)
            input.push_back(instr);
    };

    return input;
}

void
solve_1(const input_t& input)
{  
    std::array<std::array<bool, 1000>, 1000> grid;
    for (auto& e : grid)
    {
        e.fill(false);
    }

    for (const auto& e : input)
    {
        for (size_t i = e.rectangle.top_left.y; i <= e.rectangle.bottom_right.y; i++)
        {
            for (size_t j = e.rectangle.top_left.x; j <= e.rectangle.bottom_right.x; j++)
            {
                switch (e.action)
                {
                case instruction_type::toggle:
                    grid[i][j] = !grid[i][j];
                    break;
                case instruction_type::turn_on:
                    grid[i][j] = true;
                    break;
                case instruction_type::turn_off:
                    grid[i][j] = false;
                    break;
                }

            }
        }
    }

    size_t count = 0;
    for (auto& e : grid)
    {
        count += std::ranges::count(e, true);
    }

    std::cout << count << std::endl;
}

void
solve_2(const input_t& input)
{
    std::array<std::array<int64_t, 1000>, 1000> grid;
    for (auto& e : grid)
    {
        e.fill(0);
    }

    for (const auto& e : input)
    {
        for (size_t i = e.rectangle.top_left.y; i <= e.rectangle.bottom_right.y; i++)
        {
            for (size_t j = e.rectangle.top_left.x; j <= e.rectangle.bottom_right.x; j++)
            {
                switch (e.action)
                {
                case instruction_type::toggle:
                    grid[i][j] += 2;
                    break;
                case instruction_type::turn_on:
                    grid[i][j] ++;
                    break;
                case instruction_type::turn_off:
                    if (grid[i][j] >= 1)
                    {
                        grid[i][j] --;
                    }
                    break;
                
                }

            }
        }
    }

    size_t count = 0;
    for (auto& e : grid)
    {
        for (auto l : e)
            count += l;
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