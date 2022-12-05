#include "../outf.h"

#include<algorithm>
#include <vector>
#include<string>

struct input_t
{
    std::vector<std::string> stacks;

    struct move_spec_t
    {
        int64_t quantity;
        int64_t from;
        int64_t to;
    };
    std::vector<move_spec_t> moves;
};

input_t read()
{
    input_t input;
    std::string line;
    bool reading_stacks = true;
    while (std::getline(std::cin, line))
    {
        if (line.size() > 0)
        {
            if (reading_stacks)
            {
                for (size_t j = 0; j < line.size(); j++)
                {
                    char ch = line[j];
                    if (ch >= 'A' && ch <= 'Z')
                    {
                        size_t i = j / 4;
                        if (input.stacks.size() <= i)
                        {
                            input.stacks.resize(i + 1);
                        }
                        input.stacks[i] = ch + input.stacks[i];
                    }
                }
            }
            else
            {
                auto parts = split_string(line, " ");

                input.moves.emplace_back(stoll(parts[1]), stoll(parts[3]), stoll(parts[5]));
            }
        }
        else
        {
            reading_stacks = false;
        }
    }

    input_t p = input;
    return p;
}


void solve_1(const input_t& input)
{
    auto stacks = input.stacks;
    for (auto& crane_move : input.moves)
    {
        auto& from = stacks[crane_move.from - 1];
        auto& to = stacks[crane_move.to - 1];
        size_t quantity = crane_move.quantity;

        auto crates = from.substr(from.size() - quantity, quantity);
        std::reverse(crates.begin(), crates.end());
        to += crates;
        from.resize(from.size() - quantity);
    }
   
    for (auto& stack : stacks)
    {
        if (stack.size() > 0)
        {
            outf("{}", stack.back());
        }
    }
    outf("\n");

}

void solve_2(const input_t& input)
{
    auto stacks = input.stacks;
    for (auto& crane_move : input.moves)
    {
        auto& from = stacks[crane_move.from - 1];
        auto& to = stacks[crane_move.to - 1];
        size_t quantity = crane_move.quantity;

        auto crates = from.substr(from.size() - quantity, quantity);
        //std::reverse(crates.begin(), crates.end());
        to += crates;
        from.resize(from.size() - quantity);
    }

    for (auto& stack : stacks)
    {
        if (stack.size() > 0)
        {
            outf("{}", stack.back());
        }
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