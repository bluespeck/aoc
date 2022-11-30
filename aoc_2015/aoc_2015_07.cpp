#include <ranges>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <unordered_map>

enum struct operation_type_t
{
    COPY,
    NOT,
    RSHIFT,
    LSHIFT,
    OR,
    AND
};

struct operation_t
{
    std::string operand_1, operand_2;
    bool is_binary;
    operation_type_t type;
    std::string output;
};

using input_t = std::vector<operation_t>;

input_t
read_input()
{
    input_t input;

    while (std::cin)
    {
        operation_t operation;
        std::string word1, word2;
        if (std::cin >> word1 >> word2)
        {
            if (word2 == "->")
            {
                operation.is_binary = false;
                operation.type = operation_type_t::COPY;
                operation.operand_1 = word1;
                if (std::cin >> word1)
                {
                    operation.output = word1;
                    input.push_back(operation);
                    continue;
                }
                
            }
            else if(word1 == "NOT")
            {
                operation.is_binary = false;
                operation.type = operation_type_t::NOT;
                operation.operand_1 = word2;
            }
            else
            {
                
                operation.operand_1 = word1;
                
                if (word2 == "RSHIFT")
                {
                    operation.type = operation_type_t::RSHIFT;
                }
                else if (word2 == "LSHIFT")
                {
                    operation.type = operation_type_t::LSHIFT;
                }                                            
                else if (word2 == "AND")
                {
                    operation.type = operation_type_t::AND;
                }
                else if (word2 == "OR")
                {
                    operation.type = operation_type_t::OR;
                }
                
                if (std::cin >> word1)
                {
                    operation.operand_2 = word1;
                }
                operation.is_binary = true;
            }
            if (std::cin >> word1 >> word2)
            {
                operation.output = word2;
                input.push_back(operation);
            }
            
        }
    };

    return input;
}

void
solve_1(const input_t& input)
{  
    std::unordered_map<std::string, uint16_t> live_wires;

   
    for (auto& e : input)
    {
        if (e.type == operation_type_t::COPY && std::isdigit(e.operand_1[0]))
        {
            live_wires[e.output] = std::stoll(e.operand_1);
        }
    }

    bool should_continue = true;

    while (should_continue)
    {
        should_continue = false;
        for (auto& e : input)
        {
            if (!live_wires.contains(e.output))
            {
                should_continue = true;
            }

            uint16_t op1, op2;
            if (std::isalpha(e.operand_1[0]))
            {
                if (!live_wires.contains(e.operand_1))
                {
                    continue;
                }
                op1 = live_wires[e.operand_1];
            }
            else
            {
                op1 = std::stoi(e.operand_1);
            }
            
            if (e.is_binary)
            {
                if (std::isalpha(e.operand_2[0]))
                {
                    if (!live_wires.contains(e.operand_2))
                    {
                        continue;
                    }
                    op2 = live_wires[e.operand_2];
                }
                else
                {
                    op2 = std::stoi(e.operand_2);
                }
            }
            
            switch(e.type)
            {
            case operation_type_t::COPY:
                live_wires[e.output] = op1;             break;
            case operation_type_t::AND:
                live_wires[e.output] = op1 & op2;       break;
            case operation_type_t::OR:
                live_wires[e.output] = op1 | op2;       break;
            case operation_type_t::LSHIFT:
                live_wires[e.output] = op1 << op2;      break;
            case operation_type_t::RSHIFT:
                live_wires[e.output] = op1 >> op2;      break;
            case operation_type_t::NOT:
                live_wires[e.output] = ~op1;           break;
            }
        }
    }

    std::cout << live_wires["a"] << std::endl;
}

int
main()
{
    input_t input = read_input();

    solve_1(input);
    // for 2nd, change input so that b gets 3176 instead of 44430

    return 0;
}