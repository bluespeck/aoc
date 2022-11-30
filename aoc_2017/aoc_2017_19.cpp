#include "../outf.h"

#include <cctype>
#include <iostream>
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


void solve_1_2(const input_t& input)
{
    // x grows to the right
    // y grows down
    int32_t position_y = 0;
    int32_t position_x = input[position_y].find_last_not_of(' ');
    int32_t direction_x = 0;
    int32_t direction_y = 1;
    std::string letters_encountered;
    int32_t steps = 0;

    char current_symbol = input[position_y][position_x];

    for(bool reached_the_end = false; !reached_the_end;)
    {
        switch (current_symbol)
        {
        case ' ':
        {
            outf("Finished in {} steps, with these letters encountered:\n{}\n", steps, letters_encountered);
            reached_the_end = true;
            break;
        }
        case '|':
        case '-':
        {
            int32_t count = 0;
            while (current_symbol != '+' and current_symbol != ' ')
            {
                if (std::isalpha(current_symbol))
                {
                    letters_encountered += current_symbol;
                }
                position_x += direction_x;
                position_y += direction_y;
                current_symbol = input[position_y][position_x];
                count++;
            }
            steps += count;
            //char direction = direction_x == -1 ? 'L' : (direction_x == 1 ? 'R' : (direction_y == -1 ? 'U' : 'D'));
            //outf("{} {} ({} {}) count: {}{}\n", position_x, position_y, direction_x, direction_y, count, direction);
            break;
        }
        case '+':
        {
            char neighbor_right = position_x + 1 < input[position_y].size() ? input[position_y      ][position_x + 1] : ' ';
            char neighbor_left  = position_x - 1 >= 0                       ? input[position_y      ][position_x - 1] : ' ';
            char neighbor_up    = position_y - 1 >= 0                       ? input[position_y - 1  ][position_x    ] : ' ';
            char neighbor_down  = position_y + 1 < input.size()             ? input[position_y + 1  ][position_x    ] : ' ';

            if (neighbor_right != ' ' and direction_x == 0)
            {
                direction_x = 1;
                direction_y = 0;
            }
            else if (neighbor_left != ' ' and direction_x == 0)
            {
                direction_x = -1;
                direction_y = 0;
            }
            else if (neighbor_up != ' ' and direction_y == 0)
            {
                direction_x = 0;
                direction_y = -1;
            }
            else if (neighbor_down != ' ' and direction_y == 0)
            {
                direction_x = 0;
                direction_y = 1;
            }

            position_x += direction_x;
            position_y += direction_y;
            current_symbol = input[position_y][position_x];
            steps++;
            break;
        }
        default:
            break;
        }
    }


}

int main()
{
    input_t input = read();
    solve_1_2(input);

    return 0;
}