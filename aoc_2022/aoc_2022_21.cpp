#include "../outf.h"

#include <cassert>
#include <string>
#include <vector>

struct monkey_t
{
    std::string name;
    bool operation = false;
    int64_t number;
    std::string op1, op2;
    monkey_t* pop1 = nullptr;
    monkey_t* pop2 = nullptr;
    char op_type;
    bool is_human_ancestor = false;
};

using input_t = std::vector<monkey_t>;
input_t read()
{
    input_t input;
    std::string line;

    while (std::getline(std::cin, line))
    {
        monkey_t monkey;
        auto parts = split_string(line,": ");
        monkey.name = parts[0];
        if (parts[1][0] <= '9')
        {
            monkey.number = stoll(parts[1]);
            monkey.operation = false;
        }
        else
        {
            auto op_parts = split_string(parts[1]," ");
            monkey.op1 = op_parts[0];
            monkey.op_type = op_parts[1][0];
            monkey.op2 = op_parts[2];
            monkey.operation = true;
        }
        input.push_back(monkey);

    }
    return input;
}

int64_t solve_monkey_1(const input_t& input, const std::string& name)
{
    for (auto& monkey : input)
    {
        if (monkey.name == name)
        {
            if (monkey.operation)
            {
                switch (monkey.op_type)
                {
                case '+':
                    return solve_monkey_1(input, monkey.op1) + solve_monkey_1(input, monkey.op2);
                    break;
                case '-':
                    return solve_monkey_1(input, monkey.op1) - solve_monkey_1(input, monkey.op2);
                    break;
                case '*':
                    return solve_monkey_1(input, monkey.op1) * solve_monkey_1(input, monkey.op2);
                    break;
                case '/':
                    return solve_monkey_1(input, monkey.op1) / solve_monkey_1(input, monkey.op2);
                    break;
                }
                break;
            }
            else
            {
                return monkey.number;
            }
        }
    }
}

void solve_1(const input_t& input)
{
    outf("I am root yells {}.\n", solve_monkey_1(input, "root"));
}

bool build_tree(input_t& input, monkey_t* node)
{
    if (node->operation)
    {
        bool is_human_ancestor = false;
        size_t op_count = 0;
        for (auto& monkey : input)
        {
            if (monkey.name == node->op1)
            {
                node->pop1 = &monkey;
                is_human_ancestor = build_tree(input, &monkey) || is_human_ancestor;
                if (++op_count == 2)
                    break;
            }
            else if (monkey.name == node->op2)
            {
                node->pop2 = &monkey;
                is_human_ancestor = build_tree(input, &monkey) || is_human_ancestor;
                if (++op_count == 2)
                    break;
            }
        }
        node->is_human_ancestor = is_human_ancestor;
        return is_human_ancestor;
    }
    else if (node->name == "humn")
    {
        node->is_human_ancestor = true;
        return true;
    }
    else
    {
        node->is_human_ancestor = false;
        return false;
    }
}

void solve_monkey_2(const input_t& input, monkey_t* monkey, int64_t value_to_have)
{
    if (monkey->operation)
    {

        auto other_op = monkey->pop1;
        auto ancestor_op = monkey->pop2;
        if (monkey->pop1->is_human_ancestor)
        {
            assert(!monkey->pop2->is_human_ancestor);
            other_op = monkey->pop2;
            ancestor_op = monkey->pop1;
        }
        else
        {
            assert(monkey->pop2->is_human_ancestor);
        }

        switch (monkey->op_type)
        {
        case '+':
            solve_monkey_2(input, ancestor_op, value_to_have - solve_monkey_1(input, other_op->name));
            break;
        case '-':
            if (ancestor_op == monkey->pop1)
            {
                solve_monkey_2(input, ancestor_op, value_to_have + solve_monkey_1(input, other_op->name));
            }
            else
            {
                solve_monkey_2(input, ancestor_op, solve_monkey_1(input, other_op->name) - value_to_have);
            }
            break;
        case '*':
            solve_monkey_2(input, ancestor_op, value_to_have / solve_monkey_1(input, other_op->name));
            break;
        case '/':
            if (ancestor_op == monkey->pop1)
            {
                solve_monkey_2(input, ancestor_op, value_to_have * solve_monkey_1(input, other_op->name));
            }
            else
            {
                solve_monkey_2(input, ancestor_op, solve_monkey_1(input, other_op->name) / value_to_have);
            }
            break;
        case '=':
            solve_monkey_2(input, ancestor_op, solve_monkey_1(input, other_op->name));
        }
    }
    else
    {
        outf("Descendent of root yells {}.\n", value_to_have);
    }
}

void solve_2(const input_t& input_)
{
    auto input = input_;
    size_t human_index = 0;
    size_t root_index = 0;
    size_t index = 0;
    for (auto& e : input)
    {
        ++index;
        if (e.name == "root")
        {
            e.op_type = '=';
            root_index = index - 1;
        }
        else if (e.name == "humn")
        {
            human_index = index - 1;
        }
    }

    build_tree(input, &input[root_index]);
    
    solve_monkey_2(input, &input[root_index], 0);
    
}

int main()
{
    input_t input = read();
    solve_1(input);
    solve_2(input);

    return 0;
}