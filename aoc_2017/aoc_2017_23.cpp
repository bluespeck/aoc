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

void instr_set(memory_t& memory, char reg, int64_t value)
{
    assert(reg < 26);
    memory[reg] = value;
}

void instr_sub(memory_t& memory, char reg, int64_t value)
{
    assert(reg < 26);
    memory[reg] -= value;
}

void instr_mul(memory_t& memory, char reg, int64_t value)
{
    assert(reg < 26);
    memory[reg] *= value;
}


void instr_jnz(memory_t& memory, int64_t value1, int64_t value2)
{
    if (value1 != 0)
    {
        memory[instr_ptr] += value2 - 1;
    }
}

void print_instr(const instruction_t& instr)
{
    outf("{}", instr.name);
    if (instr.has_immediate1)
    { 
        outf(" {}", instr.reg1);
    }
    else
    {
        outf(" {:c}", instr.reg1 + 'a');
    }
    if (instr.has_immediate2)
    {
        outf(" {}", instr.immediate);
    }
    else
    {
        outf(" {:c}", instr.reg2 + 'a');
    }
    outf("\n");
}

void print_registry(const memory_t& memory)
{
    for (char c = 'a'; c <= 'h'; c++)
    {
        outf("{: ^16}|", c);
    }
    outf("{: ^16}|\n", "IP");
    for (char c = 'a'; c <= 'h'; c++)
    {
        outf("{:14}", memory[c - 'a']);
        outf("  |");
    }
    outf("{:14}  |\n", memory[instr_ptr]);
}

int64_t count_mul = 0;
void execute(memory_t& memory, const instruction_t& instr)
{
//    print_instr(instr);
    if (instr.name == "set")
    {
        instr_set(memory, instr.reg1, instr.has_immediate2 ? instr.immediate : memory[instr.reg2]);
    }
    else if (instr.name == "sub")
    {
        instr_sub(memory, instr.reg1, instr.has_immediate2 ? instr.immediate : memory[instr.reg2]);
    }
    else if (instr.name == "mul")
    {
        count_mul++;
        instr_mul(memory, instr.reg1, instr.has_immediate2 ? instr.immediate : memory[instr.reg2]);
    }
    else if (instr.name == "jnz")
    {
        instr_jnz(memory, instr.has_immediate1 ? instr.reg1 : memory[instr.reg1], instr.has_immediate2 ? instr.immediate : memory[instr.reg2]);
    }
//    print_registry(memory);
    memory[instr_ptr]++;
}

void exec_prog(){
    int64_t a, b, c, d, e, f, g, h;
    b = c = d = e = f = g = h = 0;
    a = 1;
    b = (84 * 100) + 100'000;
    c = b + 17'000;

    while (true)
    {
        f = 1;
        d = 2;
        do
        {
            //bool found = false;
            //e = 2;
            //do
            //{
            //    if (d * e == b)
            //    {
            //        f = 0;
            //        found = true;
            //        break;
            //    }
            //    e++;
            //} while (e != b);
            //d++;
            //if (found == true)
            //    break;
            if (b % d == 0)
            {
                f = 0;
                break;
            }
            d++;
        } while (d != b);
        if (f == 0)
        {
            h ++;
        }
        if (b == c)
        {
            outf("Value of h is {}\n", h);
            return;
        }
        b += 17;
    }
}

void solve_1(const input_t& input)
{
    count_mul = 0;
    memory_t memory = { 0 };
    int64_t executed_instruction_count = 0;
    while (true)
    {
        executed_instruction_count++;
        if (executed_instruction_count % 1'000'000 == 0)
        {
            print_registry(memory);
        }
        if (memory[instr_ptr] >= input.size())
        {
            outf("Reached the end\n");
            break;
        }
        execute(memory, input[memory[instr_ptr]]);
    }
    outf("mul was executed {} times.", count_mul);
}

void solve_2(const input_t& input)
{
    exec_prog();
}

int main()
{
    input_t input = read();
    //solve_1(input);
    solve_2(input);

    return 0;
}