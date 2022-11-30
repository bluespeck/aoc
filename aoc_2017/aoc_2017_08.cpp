#include <cstdint>
#include "x64/../../outf.h"
#include <ranges>
#include <vector>
#include <iostream>
#include <cassert>
#include <unordered_map>

enum operator_type_t
{
    inc,
    dec,
    greater,
    less,
    greater_or_equal,
    less_or_equal,
    equal,
    not_equal,
    unknown
};

std::string_view operator_strs[8] = { "inc", "dec", ">", "<", ">=", "<=", "==", "!=" };

struct instruction_t
{
    operator_type_t operator_type;
    std::string op1;
    std::string op2;
};

struct program_line_t
{
    instruction_t instr;
    instruction_t cond;
};

using input_t = std::vector<program_line_t>;

operator_type_t translate(std::string_view operator_str)
{
    auto it = std::ranges::find(operator_strs, operator_str);
    if (it == operator_strs + 8)
        return operator_type_t::unknown;
    return (operator_type_t)(it - operator_strs);
}

input_t read()
{
    input_t input;
    while (std::cin)
    {
        program_line_t line;
        auto read_instr = [](instruction_t& instr) {
            std::cin >> instr.op1;
            std::string operator_str;
            std::cin >> operator_str;
            instr.operator_type = translate(operator_str);
            std::cin >> instr.op2;
        };

        std::string if_word_to_skip;
        
        read_instr(line.instr);
        std::cin >> if_word_to_skip;
        read_instr(line.cond);
        if (line.instr.operator_type
            != operator_type_t::unknown && line.cond.operator_type
            != operator_type_t::unknown)
        {
            input.push_back(line);
        }
    }

    return input;
}

using registers_t = std::unordered_map<std::string, int64_t>;
registers_t registers;

bool check_cond(const instruction_t& instr)
{
    int64_t op1 = (std::isdigit(instr.op1[0]) || instr.op1[0] == '-' || instr.op1[0] == '+') ? std::stoll(instr.op1) : registers.at(instr.op1);
    int64_t op2 = (std::isdigit(instr.op2[0]) || instr.op2[0] == '-' || instr.op2[0] == '+') ? std::stoll(instr.op2) : registers.at(instr.op2);
    switch (instr.operator_type)
    {
    case operator_type_t::equal: return op1 == op2; break;
    case operator_type_t::not_equal: return op1 != op2; break;
    case operator_type_t::greater: return op1 > op2; break;
    case operator_type_t::less: return op1 < op2; break;
    case operator_type_t::less_or_equal: return op1 <= op2; break;
    case operator_type_t::greater_or_equal: return op1 >= op2; break;
    default: assert(false);  return false;
    }
    return false;
}

void run_instr(const instruction_t& instr)
{
    if (instr.operator_type == operator_type_t::inc)
    {
        if (!isdigit(instr.op2[0]) && instr.op2[0] != '-' && instr.op2[0] != '+')
            registers[instr.op1] += registers[instr.op2];
        else
            registers[instr.op1] += std::stoll(instr.op2);
    }
    else if (instr.operator_type == operator_type_t::dec)
    {
        if (!isdigit(instr.op2[0]) && instr.op2[0] != '-' && instr.op2[0] != '+')
            registers[instr.op1] -= registers[instr.op2];
        else
            registers[instr.op1] -= std::stoll(instr.op2);
    }
}

void solve_1_2(const input_t& input)
{
    int64_t max_val_ever = std::numeric_limits<int64_t>::min();
    std::string max_reg_name_ever = "";
    for (const auto& line : input)
    {
        if (!registers.contains(line.instr.op1))
            registers[line.instr.op1] = 0;
        if (!std::isdigit(line.instr.op2[0]) && line.instr.op2[0] != '-' && line.instr.op2[0] != '+' && !registers.contains(line.instr.op2))
            registers[line.instr.op2] = 0;
        if (!std::isdigit(line.cond.op1[0]) && line.cond.op1[0] != '-' && line.cond.op1[0] != '+' && !registers.contains(line.cond.op1))
            registers[line.cond.op1] = 0;
        if (!std::isdigit(line.cond.op2[0]) && line.cond.op2[0] != '-' && line.cond.op2[0] != '+' && !registers.contains(line.cond.op2))
            registers[line.cond.op2] = 0;
        
        if (check_cond(line.cond))
        {
            run_instr(line.instr);
            if (max_val_ever < registers[line.instr.op1])
            {
                max_val_ever = registers[line.instr.op1];
                max_reg_name_ever = line.instr.op1;
            }
        }
    }

    int64_t max_val = std::numeric_limits<int64_t>::min();
    std::string max_reg_name = "";
    for (auto& [key, val] : registers)
    {
        if (max_val < val)
        {
            max_val = val;
            max_reg_name = key;
        }
    }

    outf("Max reg value is {} = {}\n", max_reg_name, max_val);
    outf("Max reg value ever is {} = {}\n", max_reg_name, max_val_ever);
}

int main()
{
    input_t input = read();

    solve_1_2(input);

    return 0;
}
