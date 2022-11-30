#include "../outf.h"

#include <array>
#include <cassert>
#include <iostream>
#include <format>
#include <queue>
#include <string_view>
#include <string>

struct instruction_t
{
    std::string name;
    int64_t immediate;
    char reg1;
    char reg2;
    bool has_immediate1;
    bool has_immediate2;
};

using input_t = std::vector<instruction_t>;
input_t read()
{
    input_t input;
    auto parse_instruction = [](std::string_view line) -> instruction_t
    {
        instruction_t instr;
        auto substrings = split_string(line, " ");
        assert(substrings.size() >= 2 && substrings.size() <= 3);
        
        instr.name = substrings[0];
        assert(substrings[1].size() == 1);
        
        instr.has_immediate1 = substrings[1][0] <= '9';
        instr.reg1 = instr.has_immediate1 ? substrings[1][0] : substrings[1][0] - 'a';

        instr.has_immediate2 = false;

        if (substrings.size() > 2)
        {
            instr.has_immediate2 = substrings[2][0] < '9';
            instr.reg2 = substrings[2][0] - 'a';
            if (instr.has_immediate2)
            {
                instr.immediate = std::stoi(substrings[2]);
            }
        }
        return instr;
    };
    std::string line;
    while (std::getline(std::cin, line))
    {
        input.push_back(parse_instruction(line));
    }
    return input;
}

using memory_t = std::array<int64_t, 30>;

const char instr_ptr = 26;
const char played_sound = 27;
const char recovery_register = 28;

void instr_snd(memory_t & memory, char reg)
{
    memory[played_sound] = memory[reg];
}

void instr_set(memory_t & memory, char reg, int64_t value)
{
    assert(reg < 26);
    memory[reg] = value;
}

void instr_add(memory_t & memory, char reg, int64_t value)
{
    assert(reg < 26);
    memory[reg] += value;
}

void instr_mul(memory_t & memory, char reg, int64_t value)
{
    assert(reg < 26);
    memory[reg] *= value;
}

void instr_mod(memory_t & memory, char reg, int64_t value)
{
    assert(reg < 26);
    memory[reg] %= value;
}

void instr_rcv(memory_t & memory, char reg)
{
    if (memory[reg] != 0)
    {
        memory[recovery_register] = memory[played_sound];
    }
}

void instr_jgz(memory_t & memory, int64_t value1, int64_t value2)
{
    if (value1 > 0)
    {
        memory[instr_ptr] += value2 - 1;
    }
}

void execute(memory_t& memory, const instruction_t& instr)
{
    if (instr.name == "snd")
    {
        instr_snd(memory, instr.reg1);
    }
    else if (instr.name == "set")
    {
        instr_set(memory, instr.reg1, instr.has_immediate2 ? instr.immediate : memory[instr.reg2]);
    }
    else if (instr.name == "add")
    {
        instr_add(memory, instr.reg1, instr.has_immediate2 ? instr.immediate : memory[instr.reg2]);
    }
    else if (instr.name == "mul")
    {
        instr_mul(memory, instr.reg1, instr.has_immediate2 ? instr.immediate : memory[instr.reg2]);
    }
    else if (instr.name == "mod")
    {
        instr_mod(memory, instr.reg1, instr.has_immediate2 ? instr.immediate : memory[instr.reg2]);
    }
    else if (instr.name == "rcv")
    {
        instr_rcv(memory, instr.reg1);
    }
    else if (instr.name == "jgz")
    {
        instr_jgz(memory, instr.has_immediate1 ? instr.reg1 : memory[instr.reg1], instr.has_immediate2 ? instr.immediate : memory[instr.reg2]);
    }
    memory[instr_ptr]++;
}

using queue_t = std::queue<int64_t>;
using comm_queues_t = std::array<queue_t, 2>;
const char program_waiting = 27;
const char sent_count = 28;
const char program_index = 29;
const size_t program_count = 2;

void instr_snd(memory_t& memory, queue_t& queue, int64_t value)
{
    queue.push(value);
    memory[sent_count]++;
}

void instr_rcv(memory_t& memory, queue_t& queue, char reg)
{
    if (queue.empty())
    {
        memory[program_waiting] = 1;
        memory[instr_ptr] --; // repeat the receive if we are waiting
    }
    else
    {
        memory[reg] = queue.front();
        queue.pop();
        memory[program_waiting] = 0;
    }
}

void execute(memory_t& memory, comm_queues_t& comm_queues, const instruction_t& instr)
{
    if (instr.name == "snd")
    {
        instr_snd(memory, comm_queues[memory[program_index]], instr.has_immediate1 ? instr.reg1 : memory[instr.reg1]);

    }
    else if (instr.name == "set")
    {
        instr_set(memory, instr.reg1, instr.has_immediate2 ? instr.immediate : memory[instr.reg2]);
    }
    else if (instr.name == "add")
    {
        instr_add(memory, instr.reg1, instr.has_immediate2 ? instr.immediate : memory[instr.reg2]);
    }
    else if (instr.name == "mul")
    {
        instr_mul(memory, instr.reg1, instr.has_immediate2 ? instr.immediate : memory[instr.reg2]);
    }
    else if (instr.name == "mod")
    {
        instr_mod(memory, instr.reg1, instr.has_immediate2 ? instr.immediate : memory[instr.reg2]);
    }
    else if (instr.name == "rcv")
    { 
        instr_rcv(memory, comm_queues[program_count - 1 - memory[program_index]], instr.reg1);
    }
    else if (instr.name == "jgz")
    {
        instr_jgz(memory, instr.has_immediate1 ? instr.reg1 : memory[instr.reg1], instr.has_immediate2 ? instr.immediate : memory[instr.reg2]);
    }
    memory[instr_ptr]++;
}

void solve_1(const input_t& input)
{
    memory_t memory = { 0 };
    while (true)
    {
        execute(memory, input[memory[instr_ptr]]);
        if (memory[recovery_register] != 0)
        {
            outf("Recovered frequency is {}\n", memory[recovery_register]);
            break;
        }
    }
}

void solve_2(const input_t& input)
{
    std::array<memory_t, 2> memory = {};
    comm_queues_t queues = {};

    memory[0]['p' - 'a'] = 0;
    memory[0][program_index] = 0;
    memory[1]['p' - 'a'] = 1;
    memory[1][program_index] = 1;

    while (true)
    {
        bool deadlock = true;
        for (size_t i = 0; i < 2; i++)
        {
            execute(memory[i], queues, input[memory[i][instr_ptr]]);
            if (memory[i][program_waiting] == 0)
            {
                deadlock = false;
            }
        }
        if (deadlock == true)
        {
            outf("Program 1 sent {} times\n.", memory[1][sent_count]);
            break;
        }
        
    }

}

int main()
{
    input_t input = read();
    solve_1(input);
    solve_2(input);

    return 0;
}