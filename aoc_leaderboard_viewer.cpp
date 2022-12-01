#include <cstdio>
#include <array>
#include <algorithm>
#include <cassert>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "outf.h"

using Input = std::vector<std::string>;

Input read_input()
{
    Input input;
    while (std::cin)
    {
        std::string line;
        std::getline(std::cin, line);
        if (line.size())
        {
            size_t found = line.find_first_not_of(std::string{ " \t" });
            input.emplace_back(line.substr(found));
        }
    }
    return input;
}

using Time_Array = std::array<std::array<int64_t, 2>, 25>;
using Time_Board = std::unordered_map<std::string, Time_Array>;

enum class Json_Node_Type
{
    value,
    aggregate
};

struct Json_Node
{
    Json_Node_Type type;
    std::vector < std::unique_ptr<Json_Node>> subnodes;
    std::string name;
    std::string value;

};

void skip_ws(const std::string& input, int32_t& i)
{
    while (input[i] == ' ' || input[i] == '\t') i++;
}

// no array support, that is no [...]
Json_Node parse(const std::string& input, int32_t& index)
{
    Json_Node node;
    if (input[index] == '"')
    {
        index++; // starting '"'
        node.name = "";
        while (input[index] != '"' || input[index - 1] == '\\')
        {
            node.name += input[index++];
        }
        index++; // ending '"'
        skip_ws(input, index);
        assert(input[index] == ':');
        index++; // ':'
    }
    skip_ws(input, index);
    if (input[index] == '{')
    {
        node.type = Json_Node_Type::aggregate;
        index++; // begining '{'
        skip_ws(input, index);
        while (input[index] != '}')
        {
            node.subnodes.push_back(std::make_unique<Json_Node>(parse(input, index)));
            skip_ws(input, index);
            if (input[index] == ',')
            {
                index++;
            }
        }

        skip_ws(input, index);
        index++; // ending '}'
    }
    else
    {
        node.type = Json_Node_Type::value;
        if (input[index] == '"')
        {
            index++; //skip begining '"'
            node.value = "";
            while (input[index] != '"' || input[index - 1] == '\\')
            {
                node.value += input[index++];
            }
            index++; // skip ending '"'
        }
        else
        {
            node.value = "";
            while (input[index] != ' ' && input[index] != '\t' && input[index] != ',' && input[index] != '}')
            {
                node.value += input[index++];
            }
        }

        skip_ws(input, index);
        if (input[index] == ',')
        {
            index++;
        }
    }
    return node;
}

void solve(const Input& _input)
{
    std::string input = "";
    for (auto e : _input)
        input += e;

    int32_t i = 0;
    Json_Node root = parse(input, i);
    
    Time_Board time_board;

    for (auto& root_subnode : root.subnodes)
    {
        if (root_subnode->name == "members")
        {
            for (auto& person : root_subnode->subnodes)
            {
                std::string person_name = "";
                Time_Array person_daily_scores = {};
                for (auto& field : person->subnodes)
                {
                    if (field->name == "name")
                    {
                        person_name = field->value;
                    }
                    else if (field->name == "completion_day_level")
                    {
                        for (auto& day_field : field->subnodes)
                        {
                            int32_t day = std::stol(day_field->name);
                            for (auto& score : day_field->subnodes)
                            {
                                int32_t part = std::stol(score->name);
                                if (score->subnodes.size() == 1)
                                {
                                    person_daily_scores[day - 1][part - 1] = std::stoll(score->subnodes[0]->value);
                                }
                                else if (score->subnodes.size() == 2)
                                {
                                    if (score->subnodes[0]->name == "get_star_ts")
                                    {
                                        person_daily_scores[day - 1][part - 1] = std::stoll(score->subnodes[0]->value);
                                    }
                                    else if (score->subnodes[1]->name == "get_star_ts")
                                    {
                                        person_daily_scores[day - 1][part - 1] = std::stoll(score->subnodes[1]->value);
                                    }
                                    else
                                    {
                                        outf("-------- unhandled json format time node name changed ----");
                                    }
                                }
                                else {
                                    outf("-------- unhandled json format ----");
                                }
                            }
                        }
                    }
                }
                if (!time_board.contains(person_name))
                {
                    time_board[person_name] = person_daily_scores;
                }
                else
                {
                    for (size_t i = 0; i < 25;i++)
                    {
                        auto [v1, v2] = person_daily_scores[i];
                        if (v1 != 0)
                            time_board[person_name][i][0] = v1;
                        if (v2 != 0)
                            time_board[person_name][i][1] = v2;
                    }
                }
            }
        }
    }
    
    for (int32_t day = 1; day <= 25; day++)
    {
        struct Day_Result
        {
            std::string name;
            int64_t prob1_time;
            int64_t prob2_time;
        };
        std::vector<Day_Result> day_results;
        for (auto& [name, times] : time_board)
        {
            day_results.push_back({ name, times[day - 1][0], times[day - 1][1] });
        }

        {
            outf("\x1b[0m");
            outf("\nDay {:2} part {} results: =============================== \n", day, 1);
            int32_t count = 0;
            std::sort(day_results.begin(), day_results.end(), [](auto a, auto b) { return a.prob1_time < b.prob1_time; });
            for (auto& e : day_results)
            {
                if (e.prob1_time != 0)
                {
                    if (count % 2 == 0)
                    {
                        outf("\x1b[38;5;179m");
                    }
                    else
                    {
                        outf("\x1b[38;5;209m");
                    }
                    outf("{:3} {:32}", ++count, e.name);
                    std::cout << " " << std::put_time(std::localtime(&e.prob1_time), "%H:%M:%S %y.%m.%d") << "\n";
                }
            }
        }
        {
            outf("\x1b[0m");
            outf("\nDay {:2} part {} results: ------------------------------- \n", day, 2);
            int32_t count = 0;
            std::sort(day_results.begin(), day_results.end(), [](auto a, auto b) { return a.prob2_time < b.prob2_time; });
            for (auto& e : day_results)
            {
                if (e.prob2_time != 0)
                {
                    if (count % 2 == 0)
                    {
                        outf("\x1b[38;5;179m");
                    }
                    else
                    {
                        outf("\x1b[38;5;209m");
                    }
                    outf("{:3} {:32}", ++count, e.name);
                    std::cout << " " << std::put_time(std::localtime(&e.prob2_time), "%H:%M:%S %y.%m.%d") << "\n";
                }
            }
        }
    }
}

int main()
{
    auto input = read_input();
    solve(input);

    return 0;
}