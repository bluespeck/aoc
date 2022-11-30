#include <cstdint>
#include "outf.h"
#include <ranges>
#include <unordered_map>
#include <cassert>

struct program_t
{
    std::string name;
    int32_t weight;
    std::vector<std::string> sub_progs;
};

using input_t = std::vector<program_t>;
input_t read()
{
    input_t input;
    while (std::cin)
    {
        program_t prog;
        std::cin >> prog.name;
        std::cin.ignore(2);
        std::cin >> prog.weight;
        std::string rest_of_line;
        std::getline(std::cin, rest_of_line);
        if (rest_of_line.size() > 1)
        {
            prog.sub_progs = split_string(rest_of_line.substr(5), ", ");
        }
        input.push_back(prog);
    }

    return input;
}

std::string solve_1(const input_t& input)
{
    for (const auto& prog : input)
    {
        bool found_root = true;
        for (const auto& other_prog : input)
        {
            if (std::ranges::find(other_prog.sub_progs, prog.name) != other_prog.sub_progs.end())
            {
                found_root = false;
                break;
            }
        }
        if (found_root)
        {
            outf("Found main prog {} .\n", prog.name);
            return prog.name;
        }
    }
    return "bad input.\n";
}
    
using weight_map_t = std::unordered_map<std::string, uint32_t>;

const program_t& find_program(const input_t& input, std::string_view prog_name)
{
    auto it = std::ranges::find_if(input, [&prog_name](const program_t& a) { return a.name == prog_name; });
    assert(it != input.end());
    return *it;
}

uint32_t compute_tower_weight(const input_t& input, weight_map_t& weight_map, std::string_view prog_name) 
{
    uint32_t tower_weight = 0;
    auto prog = find_program(input, prog_name);
    tower_weight = prog.weight;

    for (const auto& sub_prog : prog.sub_progs)
    {
        tower_weight += compute_tower_weight(input, weight_map, sub_prog);
    }
    weight_map[std::string{ prog_name }] = tower_weight;
    return tower_weight;
};

uint32_t find_wrong_weight(const input_t& input, const weight_map_t& weight_map, std::string_view prog_name, bool& found)
{
    const program_t& prog = find_program(input, prog_name);
    uint32_t weight = 0;
    std::string good_weight_name{ prog_name };
    uint32_t good_weight = 0;

    if (prog.sub_progs.size() > 0)
    {
        if (prog.sub_progs.size() == 1)
            return find_wrong_weight(input, weight_map, prog.sub_progs[0], found);
        else if (prog.sub_progs.size() == 2)
        {
            uint32_t weight_diff = find_wrong_weight(input, weight_map, prog.sub_progs[0], found);
            if (weight_diff == 0)
                weight_diff = find_wrong_weight(input, weight_map, prog.sub_progs[1], found);
            return weight_diff;
        }
        else
        {
            
            for (const auto& sub_prog1 : prog.sub_progs)
            {
                for (const auto& sub_prog2 : prog.sub_progs)
                {
                    if (sub_prog1 != sub_prog2)
                    {
                        uint32_t weight_1 = weight_map.at(sub_prog1);
                        uint32_t weight_2 = weight_map.at(sub_prog2);
                        if (weight_1 == weight_2)
                        {
                            good_weight = weight_1;
                            good_weight_name = sub_prog1;
                            break;

                        }
                    }
                }
                if (good_weight != 0)
                    break;
            }
            for (const auto& sub_prog : prog.sub_progs)
            {
                uint32_t weight = weight_map.at(sub_prog);
                if (weight != good_weight)
                {
                    // found the bad one.
                    uint32_t weight_diff = find_wrong_weight(input, weight_map, sub_prog, found);
                    if (weight_diff == 0 && !found)
                    {
                        const program_t& input_prog = find_program(input, sub_prog);
                        weight_diff = input_prog.weight - (weight_map.at(sub_prog) - good_weight);
                        //for (auto& prgn : prog.sub_progs)
                        //{
                        //    outf("{}, {}\n", prgn, weight_map.at(prgn));
                        //}
                        outf("Weight of {} needs to be {}.", sub_prog, weight_diff);
                        found = true;
                        return weight_diff;
                    }
                }
            }
        }
    }
    return 0;
}

void solve_2(const input_t& input)
{ 
    auto root_prog = solve_1(input);


    weight_map_t weight_map;
    compute_tower_weight(input, weight_map, root_prog);
    bool found = false;
    find_wrong_weight(input, weight_map, root_prog, found);
}

int main()
{
    input_t input = read();

    solve_2(input);

    return 0;
}