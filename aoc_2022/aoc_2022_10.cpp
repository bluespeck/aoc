#include "../outf.h"

#include <string>
#include <vector>

struct instruction_t
{
    int opcode;
    int64_t op1;
};

using input_t = std::vector<instruction_t>;
input_t read()
{
    input_t input;
    std::string line;
    while (std::getline(std::cin, line))
    {
        auto parts = split_string(line, " ");
        if (parts.size() == 2)
        {
            input.emplace_back(1, stoll(parts[1]));
        }
        else
        {
            input.emplace_back(0, 0);
        }
    }

    return input;
}


void solve_1(const input_t& input)
{
    int64_t reg_x = 1;
    int64_t cycle = 1;
    int64_t acc = 0;

    bool mid_instr = false;
    int opcode = -1;
    int64_t op1 = -1;
    size_t ip = 0;

    while (ip < input.size())
    {
        if (!mid_instr)
        {
            opcode = input[ip].opcode;
            op1 = input[ip].op1;
        }
        
        if (cycle == 20 || cycle == 60 || cycle == 100 || cycle == 140 || cycle == 180 || cycle == 220)
        {
            acc += reg_x * cycle;
        }
        
        if (opcode == 1)
        {
            if (mid_instr == false)
            {
                mid_instr = true;
            }
            else
            {
                reg_x += op1;
                mid_instr = false;
            }
        }
        cycle++;
        if (!mid_instr)
        {
            ip++;
        }
    }
    outf("{}\n", acc);
}

void solve_2(const input_t& input)
{
    int64_t reg_x = 1;
    int64_t cycle = 1;

    bool mid_instr = false;
    int opcode = -1;
    int64_t op1 = -1;
    size_t ip = 0;

    std::vector<std::string > pixels;
    pixels.resize(6, std::string(40,'.'));
    
    while (ip < input.size())
    {
        size_t current_h_pixel_pos = (cycle - 1) % 40;
        size_t current_v_pixel_pos = (cycle - 1) / 40;
        if (current_h_pixel_pos == reg_x - 1 || current_h_pixel_pos == reg_x || current_h_pixel_pos == reg_x + 1)
        {
            pixels[current_v_pixel_pos][current_h_pixel_pos] = '#';
        }

        if (!mid_instr)
        {
            opcode = input[ip].opcode;
            op1 = input[ip].op1;
        }

        if (opcode == 1)
        {
            if (mid_instr == false)
            {
                mid_instr = true;
            }
            else
            {
                reg_x += op1;
                mid_instr = false;
            }
        }
        cycle++;
        if (!mid_instr)
        {
            ip++;
        }
    }

    for (auto& e : pixels)
    {
        outf("{}\n", e);
    }
    outf("\n");
}

int main()
{
    input_t input = read();
    solve_1(input);
    solve_2(input);

    return 0;
}