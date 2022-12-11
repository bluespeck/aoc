#include "../outf.h"

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

constexpr bool show_diagnostic_messages = true;

template <typename ...Args>
void doutf(const std::_Fmt_string<Args...> format_str, Args&&... args)
{
    if (show_diagnostic_messages)
    {
        outf(format_str, std::forward<Args>(args)...);
    }
}

struct monkey_t
{
    std::vector<int64_t> items;
    char operation_type;
    std::string op1, op2;
    int64_t test_value;
    size_t throw_true;
    size_t throw_false;
    size_t inspection_counter = 0;
};

using input_t = std::vector<monkey_t>;
input_t read()
{
    input_t input;
    std::string line;
    size_t current_monkey_index = 0;
    while (std::getline(std::cin, line))
    {
        monkey_t monkey;
        std::getline(std::cin, line);
        {
            auto parts1 = split_string(line, ":");
            auto parts2 = split_string(parts1[1], ",");
            for (auto& e : parts2)
            {
                monkey.items.push_back(stoll(e));
            }
        }
        std::getline(std::cin, line);
        {
            auto parts = split_string(line, " ");
            monkey.operation_type = parts[6][0];
            monkey.op1 = parts[5];
            monkey.op2 = parts[7];
        }
        std::getline(std::cin, line);
        {
            auto parts = split_string(line, " ");
            monkey.test_value = std::stoll(parts.back());
            
        }
        std::getline(std::cin, line);
        {
            auto parts = split_string(line, " ");
            monkey.throw_true = std::stoll(parts.back());
        }
        std::getline(std::cin, line);
        {
            auto parts = split_string(line, " ");
            monkey.throw_false = std::stoll(parts.back());
        }
        if (std::getline(std::cin, line))
        {
            assert(line.empty());
        }
        input.push_back(monkey);
    }

    return input;
}

void solve_1(const input_t& input)
{
    auto input_c = input;
    size_t round = 0;
    
    while (true)
    {
        round++;
        if (round == 21)
        {
            std::sort(input_c.begin(), input_c.end(), [](const auto& m1, const auto& m2)
                {
                    return m1.inspection_counter > m2.inspection_counter;
                });
            outf("Monkey business level after 20 rounds is {}.\n\n", input_c[0].inspection_counter * input_c[1].inspection_counter);
            break;
        }
        size_t index = 0;
        for (auto& monkey : input_c)
        {
            doutf("Monkey {}:\n", index++);
            for (size_t i = 0; i < monkey.items.size(); i++)
            {
                monkey.inspection_counter++;

                auto& item = monkey.items[i];
                doutf("  Monkey inspects an item with a worry level of {}.\n", item);
                
                int64_t op2 = 0;
                if (monkey.op2 == "old")
                    op2 = item;
                else
                    op2 = stoll(monkey.op2);

                if (monkey.operation_type == '+')
                {
                    item = item + op2;
                    doutf("    Worry level increases by {} to {}.\n", op2, item);
                }
                else
                {
                   item = item * op2;
                   doutf("    Worry level is multiplied by  {} to {}.\n", op2, item);
                }

                item = item / 3;
                doutf("    Monkey gets bored with item. Worry level is divided by {} to {}.\n", 3, item);

                if (item % monkey.test_value != 0)
                {
                    doutf("    Current worry level is not divisible by {}.\n", monkey.test_value);
                    doutf("    Item with worry level {} is thrown to monkey {}.\n", item, monkey.throw_false);
                    input_c[monkey.throw_false].items.push_back(item);
                }
                else
                {
                    doutf("    Current worry level is divisible by {}.\n", monkey.test_value);
                    doutf("    Item with worry level {} is thrown to monkey {}.\n", item, monkey.throw_true);
                    input_c[monkey.throw_true].items.push_back(item);
                }

                for (size_t j = i; j < monkey.items.size() - 1; j++)
                {
                    monkey.items[j] = monkey.items[j + 1];
                }
                monkey.items.resize(monkey.items.size() - 1);
                i--;
            }
        }
    }
}

void solve_2(const input_t& input)
{
    auto input_c = input;
    size_t round = 0;

    int64_t least_common_multiple_of_the_test_values = 1;
    // assuming they're prime numbers ^^'
    for (auto& monkey : input_c)
    {
        least_common_multiple_of_the_test_values *= monkey.test_value;
    }

    while (true)
    {
        round++;
        
        if (round == 10001)
        {
            std::sort(input_c.begin(), input_c.end(), [](const auto& m1, const auto& m2)
                {
                    return m1.inspection_counter > m2.inspection_counter;
                });
            outf("Monkey business level after 10000 rounds is {}.\n\n", input_c[0].inspection_counter * input_c[1].inspection_counter);
            break;
        }
        size_t index = 0;
        for (auto& monkey : input_c)
        {
            for (size_t i = 0; i < monkey.items.size(); i++)
            {
                monkey.inspection_counter++;

                auto& item = monkey.items[i];

                int64_t op2 = 0;
                if (monkey.op2 == "old")
                    op2 = item;
                else
                    op2 = stoll(monkey.op2);

                if (monkey.operation_type == '+')
                {
                    item = item + op2;
                }
                else
                {
                    item = item * op2;
                }

                item = item % least_common_multiple_of_the_test_values;

                if (item % monkey.test_value != 0)
                {
                    input_c[monkey.throw_false].items.push_back(item);
                }
                else
                {
                    input_c[monkey.throw_true].items.push_back(item);

                }

                for (size_t j = i; j < monkey.items.size() - 1; j++)
                {
                    monkey.items[j] = monkey.items[j + 1];
                }
                monkey.items.resize(monkey.items.size() - 1);
                i--;
            }
        }

        if (round == 20 || round % 1000 == 0)
        {
            doutf("Round {}:\n", round);
            for (auto& monkey : input_c)
            {
                doutf("{} and holding ", monkey.inspection_counter);
                for (auto& item : monkey.items)
                {
                    doutf("{} ", item);
                }
                doutf("\n");
            }
            doutf("\n");
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