#include <array>
#include <cstdint>
#include "../outf.h"
#include <ranges>
#include <vector>
#include <iostream>
#include <cassert>
#include <unordered_map>

struct instruction_t
{
    enum class type_t
    {
        spin,
        exchange,
        partner
    } type;
    int32_t i1, i2;
    char p1, p2;
};

using input_t = std::vector<instruction_t>;

input_t read()
{
    input_t input;
    
    std::string line;
    std::getline(std::cin, line);

    auto move_strs = split_string(line, ",");

    for (auto str : move_strs)
    {
        assert(str.size() >= 2);

        instruction_t instr;
        switch (str[0])
        {
            case 's':
            {
                instr.type = instruction_t::type_t::spin;
                instr.i1 = std::stol(str.substr(1));
                break;
            }
            case 'x':
            {
                instr.type = instruction_t::type_t::exchange;
                auto position_strs = split_string(str.substr(1), "/");
                assert(position_strs.size() == 2);
                instr.i1 = std::stol(position_strs.front());
                instr.i2 = std::stol(position_strs.back());
                break;
            }
            case 'p':
            {
                instr.type = instruction_t::type_t::partner;
                assert(str.size() == 4);
                instr.p1 = str[1];
                instr.p2 = str[3];
                break;
            }
        }
        input.push_back(instr);
    }

    return input;
}

void solve_1(const input_t& input)
{
    std::array<char, 16> partners;
    for (char i = 0; i < partners.size(); i++)
    {
        partners[i] = 'a' + i;
    }

    for (auto& instr : input)
    {
        switch (instr.type)
        {
        case instruction_t::type_t::spin:
        {
            std::rotate(partners.begin(), partners.begin() + (partners.size() - instr.i1), partners.end());
            break;
        }
        case instruction_t::type_t::exchange:
        {
            std::swap(partners[instr.i1], partners[instr.i2]);
            break;
        }
        case instruction_t::type_t::partner:
        {
            auto it1 = std::find(partners.begin(), partners.end(), instr.p1);
            auto it2 = std::find(partners.begin(), partners.end(), instr.p2);
            std::iter_swap(it1, it2);
            break;
        }
        }

    }

    for (char c : partners)
        outf("{}", c);
    std::cout << "\n";
}

void solve_2(const input_t& input)
{
    std::vector<std::array<char, 16>> past_partners;
    std::array<char, 16> partners;
    for (char i = 0; i < partners.size(); i++)
    {
        partners[i] = 'a' + i;
    }

    for (int32_t i = 0; i < 1'000'000'000; i++)
    {
        for (auto& instr : input)
        {
            switch (instr.type)
            {
            case instruction_t::type_t::spin:
            {
                std::rotate(partners.begin(), partners.begin() + (partners.size() - instr.i1), partners.end());
                break;
            }
            case instruction_t::type_t::exchange:
            {
                std::swap(partners[instr.i1], partners[instr.i2]);
                break;
            }
            case instruction_t::type_t::partner:
            {
                auto it1 = std::find(partners.begin(), partners.end(), instr.p1);
                auto it2 = std::find(partners.begin(), partners.end(), instr.p2);
                std::iter_swap(it1, it2);
                break;
            }
            }

        }
        bool period_found = false;
        for (auto p : past_partners)
        {
            if (std::equal(p.begin(), p.end(), partners.begin()))
            {
                period_found = true;
                break;
            }
        }
        if (!period_found)
        {
            past_partners.push_back(partners);
        }
        else
        {
            break;
        }
    }

    for (char c : past_partners[1'000'000'000 % past_partners.size() - 1])
        outf("{}", c);
    std::cout << "\n";
}

int main()
{
    input_t input = read();
    solve_1(input);
    solve_2(input);

    return 0;
}
