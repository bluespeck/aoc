#include "../outf.h"

#include <algorithm>
#include <string>
#include <vector>

struct value_t
{
    int value_type = 0;
    int64_t value;
    std::vector<value_t*> list;
};

struct list_pairs_t
{
    std::vector<value_t*> left;
    std::vector<value_t*> right;
};

value_t* parse(const std::string& input, size_t &pos)
{
    if (input[pos] == '[')
    {
        pos++;
        value_t* val = new value_t;
        val->value_type = 1;

        while (input[pos] != ']')
        {
            if (input[pos] == ',')
            {
                pos++;
            }
            else
            {
                val->list.push_back(parse(input, pos));
            }
        }
        pos++;
        return val;
    }
    else
    {
        size_t new_pos = input.find_first_of("[],", pos + 1);
        std::string value_str = input.substr(pos, new_pos);
        pos = new_pos;
        return new value_t{ 0, std::stoll(value_str) };
    }
}

void delete_value(value_t* value)
{
    if (value->value_type == 1)
    {
        for (auto& e : value->list)
        {
            delete_value(e);
        }
    }
    delete value;
}

using input_t = list_pairs_t;
input_t read()
{
    input_t input;
    std::string line;

    while (std::getline(std::cin, line))
    {
        std::string left = line;

        std::getline(std::cin, line);
        std::string right = line;
        
        size_t start_pos = 0;
        input.left.push_back(parse(left, start_pos));
        start_pos = 0;
        input.right.push_back(parse(right, start_pos));
        if (!std::getline(std::cin, line))
        {
            break;
        }
    }

    return input;
}

int value_order(value_t* left, value_t* right)
{
    if (left->value_type == right->value_type)
    {
        if (left->value_type == 0)
        {
            if (left->value != right->value)
            {
                return left->value < right->value ? -1 : 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            for (size_t i = 0;; i++)
            {
                if (i >= left->list.size() && i>= right->list.size())
                {
                    return 0;
                }
                else if (i >= left->list.size())
                {
                    return -1;
                }
                else if(i >= right->list.size())
                { 
                    return 1;
                }
                else
                {
                    if (i < left->list.size())
                    {
                        auto order = value_order(left->list[i], right->list[i]);
                        if (order != 0)
                            return order;
                    }
                }
            }
            return 0;
        }
    }
    else
    {
        value_t val;
        val.value_type = 1;
        if (left->value_type == 0)
        {
            val.list.push_back(left);
            return value_order(&val, right);
        }
        else
        {
            val.list.push_back(right);
            return value_order(left, &val);
        }
    }

    return 0;
}

void solve_1(const input_t& input)
{
    auto& pairs = input;
    size_t sum = 0;
    for (size_t i = 0; i < pairs.left.size(); i++)
    {
        if (value_order(pairs.left[i], pairs.right[i]) < 0)
        {
            sum += i+1;
        }
    }
    outf("{}\n", sum);
}

void print_value(value_t* value, int level = 0)
{
    if (value->value_type == 0)
    {
        outf("{}", value->value);
    }
    else
    {
        outf("[");
        for (auto& e : value->list)
        {
            print_value(e, level + 1);
            outf(" ");
        }
        outf("]");
    }
    if (level == 0)
    {
        outf("\n");
    }
}

void solve_2(const input_t& input)
{
    auto& pairs = input;

    std::vector<value_t*> vals;
    for (auto& e : pairs.left)
    {
        vals.push_back(e);

    }
    for (auto& e : pairs.right)
    {
        vals.push_back(e);
    }

    size_t pos = 0;
    value_t* div1 = parse("[[6]]", pos);
    pos = 0;
    value_t* div2 = parse("[[2]]", pos);

    vals.push_back(div1);
    vals.push_back(div2);

    std::sort(vals.begin(), vals.end(), [](auto& v1, auto& v2) { return value_order(v1, v2) <= 0; });

    size_t prod = 1;
    for (size_t i = 0; i< vals.size(); i++)
    {
        // print_value(vals[i]);
        auto& e = vals[i];
        if (value_order(e, div1) == 0 || value_order(e, div2) == 0)
        {
            prod *= i + 1;
        }

    }
    
    outf("{}\n", prod);

    delete_value(div1);
    delete_value(div2);
}

int main()
{
    input_t input = read();
    solve_1(input);
    solve_2(input);
    // cleanup
    for (auto& e : input.left)
    {
        delete_value(e);
    }
    for (auto& e : input.right)
    {
        delete_value(e);
    }
    return 0;
}