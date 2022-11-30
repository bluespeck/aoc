#include "../outf.h"

#include <array>
#include <cassert>
#include <iostream>
#include <format>
#include <span>
#include <string>
#include <vector>

struct sm_state_config_t
{
    char state;
    struct instructions_t
    {
        signed char what_to_write;
        signed char direction; //(1 or -1)
        char new_state;
    };
    std::array<instructions_t, 2> on_value;
};

struct config_t
{
    char initial_state;
    size_t checksum_steps;
    std::vector<sm_state_config_t> state_configs;
};

using input_t = config_t;
input_t read()
{
    input_t input;
    std::string line;
    if(std::getline(std::cin, line))
    {
        input.initial_state = line[line.size() - 2];

        std::getline(std::cin, line);
        input.checksum_steps = std::stoull( split_string(line, " ")[5]);


        while (std::getline(std::cin, line))
        {
            sm_state_config_t state_config;
            
            std::getline(std::cin, line);
            state_config.state = line[line.size() - 2];
            
            for (int i = 0; i < 2; i++)
            {
                std::getline(std::cin, line);
                char current_case = line[line.size() - 2];
                auto& on_value = state_config.on_value[current_case - '0'];

                std::getline(std::cin, line);
                on_value.what_to_write = line[line.size() - 2] - '0';

                std::getline(std::cin, line);
                on_value.direction = line[line.size() - 3] == 'h' ? 1 : -1;

                std::getline(std::cin, line);
                on_value.new_state = line[line.size() - 2];
            }
            input.state_configs.push_back(state_config);
        }
    }
    else
    {
        outf("No input!\n");
    }
    
    return input;
}


void solve_1(const input_t& )
{
    std::vector<signed char> tape(5, 0);
    size_t cursor = 2;
    char state = 'A';
    size_t checksum_steps = 12683008;
    size_t current_step = 0;
    auto move_cursor_left = [&cursor, &tape]()
    {
        if (cursor == 0)
        {
            tape.insert(tape.begin(), { 0,0,0,0,0 });
            cursor += 5;
        }
        --cursor;
    };
    auto move_cursor_right = [&cursor, &tape]()
    {
        if (cursor == tape.size() - 1)
        {
            tape.insert(tape.end(), { 0,0,0,0,0 });
        }
        ++cursor;
    };
    while (current_step++ < checksum_steps)
    {
        switch (state)
        {
        case 'A':
            if (tape[cursor] == 0)
            {
                tape[cursor] = 1;
                move_cursor_right();
                state = 'B';
            }
            else
            {
                tape[cursor] = 0;
                move_cursor_left();
                state = 'B';
            }
            break;
        case 'B':
            if (tape[cursor] == 0)
            {
                tape[cursor] = 1;
                move_cursor_left();
                state = 'C';
            }
            else
            {
                tape[cursor] = 0;
                move_cursor_right();
                state = 'E';
            }
            break;
        case 'C':
            if (tape[cursor] == 0)
            {
                tape[cursor] = 1;
                move_cursor_right();
                state = 'E';
            }
            else
            {
                tape[cursor] = 0;
                move_cursor_left();
                state = 'D';
            }
            break;
        case 'D':
            if (tape[cursor] == 0)
            {
                tape[cursor] = 1;
                move_cursor_left();
                state = 'A';
            }
            else
            {
                tape[cursor] = 1;
                move_cursor_left();
                state = 'A';
            }
            break;
        case 'E':
            if (tape[cursor] == 0)
            {
                tape[cursor] = 0;
                move_cursor_right();
                state = 'A';
            }
            else
            {
                tape[cursor] = 0;
                move_cursor_right();
                state = 'F';
            }
            break;
        case 'F':
            if (tape[cursor] == 0)
            {
                tape[cursor] = 1;
                move_cursor_right();
                state = 'E';
            }
            else
            {
                tape[cursor] = 1;
                move_cursor_right();
                state = 'A';
            }
            break;
        default:
            assert(false && "Invalid state!");
            break;
        }
    }
    outf("Checksum is {}\n", std::count(tape.begin(), tape.end(), 1));
}

void solve_2(const input_t& input)
{
    std::vector<signed char> tape(5, 0);
    size_t cursor = 2;
    char state = input.initial_state;
    size_t checksum_steps = input.checksum_steps;
    size_t current_step = 0;
    auto move_cursor_left = [&cursor, &tape]()
    {
        if (cursor == 0)
        {
            tape.insert(tape.begin(), { 0,0,0,0,0 });
            cursor += 5;
        }
        --cursor;
    };
    auto move_cursor_right = [&cursor, &tape]()
    {
        if (cursor == tape.size() - 1)
        {
            tape.insert(tape.end(), { 0,0,0,0,0 });
        }
        ++cursor;
    };
    while (current_step++ < checksum_steps)
    {
        for (auto& state_config : input.state_configs)
        {
            if (state_config.state == state)
            {
                auto cursor_value = tape[cursor];
                tape[cursor] = state_config.on_value[cursor_value].what_to_write;
                if (state_config.on_value[cursor_value].direction > 0) move_cursor_right();
                else move_cursor_left();
                state = state_config.on_value[cursor_value].new_state;
                break;
            }
        }
    }

    outf("Checksum is {}\n", std::count(tape.begin(), tape.end(), 1));
}

int main()
{
    input_t input = read();
    solve_1(input);
    solve_2(input);

    return 0;
}