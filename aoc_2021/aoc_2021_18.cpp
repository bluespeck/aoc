#include <cassert>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include "outf.h"

const bool are_enabled_debug_messages = false;

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
            input.push_back(line);
        }
    }
    return input;
}

struct Number
{
    int64_t value;
    std::vector<Number*> children;

    Number() = default;
    Number(int64_t val) : value{ val } {}
    Number(const Number&) = default;
    Number(Number&&) = default;
    Number& operator=(Number&&) = default;
    ~Number() { for (auto& e : children) delete e; }
};

using Root_Left_Right_List = std::list<Number*>;

Number parse_number(const std::string& num_str, size_t& i)
{
    Number number;
    if (num_str[i] == '[')
    {
        i++; // open bracket
        number.children.push_back(new Number(parse_number(num_str, i)));
        i++; // comma
        number.children.push_back(new Number(parse_number(num_str, i)));
        i++; // closed bracket
    }
    else
    {
        std::string value_str = "";
        while (num_str[i] >= '0' && num_str[i] <= '9')
        {
            value_str.push_back(num_str[i]);
            i++;
        }
        number.value = std::stoul(value_str);
    }
    return number;
}

bool explode(Number& number, Root_Left_Right_List& rlr_list, int64_t level = 0)
{
    if (number.children.size() > 0)
    {
        if (level == 4)
        {
            {
                auto it = std::find(rlr_list.begin(), rlr_list.end(), number.children[0]);
                if (it != rlr_list.end())
                {
                    if (it != rlr_list.begin())
                    {
                        auto it_left = it;
                        it_left--;
                        (*it_left)->value += number.children[0]->value;
                    }

                    {
                        it++; // go to right child value
                        auto it_right = it;
                        it_right++;
                        if (it_right != rlr_list.end())
                        {
                            (*it_right)->value += number.children[1]->value;
                        }
                    }
                }
            }
            delete number.children[0];
            delete number.children[1];
            number.children.clear();
            number.value = 0;
            return true;
        }
        else
        {
            auto result = false;
            for (auto& e : number.children)
            {
                result = result || explode(*e, rlr_list, level + 1);
            }
            return result;
        }
    }
    return false;
}

bool split(Number& number)
{
    if (number.children.size() == 0)
    {
        if (number.value >= 10)
        {
            number.children.push_back(new Number{ number.value / 2 });
            number.children.push_back(new Number{ (number.value + 1) / 2 });
            return true;
        }
        else
            return false;
    }
    else
    {
        bool result = false;
        for (auto& e : number.children)
        {
            result = result || split(*e);
        }
        return result;
    }

    return false;
}

void make_root_left_right_list(Number* number, Root_Left_Right_List& rlr_list)
{
    if (number->children.size() == 0)
    {
        rlr_list.push_back(number);
    }
    else
    {
        for (auto& e : number->children)
        {
            make_root_left_right_list(e, rlr_list);
        }
    }
}

void print_root_left_right_rec(const Number& number)
{
    if (number.children.size() == 0)
    {
        outf("{}", number.value);
    }
    else
    {
        outf("[");
        print_root_left_right_rec(*number.children[0]);
        outf(",");
        print_root_left_right_rec(*number.children[1]);
        outf("]");
    }
}

void print_root_left_right(const Number& number)
{
    print_root_left_right_rec(number);
    outf("\n");
}

void reduce(Number& root_number)
{
    if (are_enabled_debug_messages)
    {
        outf("Reducing:");
        print_root_left_right(root_number);
    }
    while (true)
    {
        Root_Left_Right_List rlr_list;
        make_root_left_right_list(&root_number, rlr_list);

        if (explode(root_number, rlr_list))
        {
            if (are_enabled_debug_messages)
            {
                outf("Exploded to ");
                print_root_left_right(root_number);
            }
        }
        else if (split(root_number))
        {
            if (are_enabled_debug_messages)
            {
                outf("Split to ");
                print_root_left_right(root_number);
            }
        }
        else break;
    }
    if (are_enabled_debug_messages)
    {
        outf("============\n");
    }
}

Number deep_copy_number(const Number& number)
{
    Number num;
    if (number.children.size() > 0)
    {
        num.children.push_back(new Number(deep_copy_number(*number.children[0])));
        num.children.push_back(new Number(deep_copy_number(*number.children[1])));
    }
    else
    {
        num.value = number.value;
    }

    return num;
}

Number add_numbers(Number& n1, Number& n2)
{
    Number result;
    result.children.push_back(new Number(deep_copy_number(n1)));
    result.children.push_back(new Number(deep_copy_number(n2)));
    reduce(result);
    return result;
}

int64_t magnitude(Number& number)
{
    if (number.children.size() == 0)
    {
        return number.value;
    }
    else
    {
        return 3 * magnitude(*number.children[0]) + 2 * magnitude(*number.children[1]);
    }
}

void solve_1(const Input& input)
{
    outf("\nPart {}: ", 1);
    std::vector<Number> numbers;
    numbers.reserve(input.size());
    for (auto& e : input)
    {
        size_t index = 0;
        numbers.push_back(parse_number(e, index));
        
        if (are_enabled_debug_messages)
        {
            print_root_left_right(numbers.back());
        }
    }
    if (are_enabled_debug_messages)
    {
        outf("---------\n");
    }

    auto sum = deep_copy_number(numbers[0]);
    if (are_enabled_debug_messages)
    {
        print_root_left_right(sum);
    }
    
    for (size_t i = 1; i < numbers.size(); i++)
    {
        if (are_enabled_debug_messages)
        {
            outf("+ ");
            print_root_left_right(numbers[i]);
        }
        {
            auto old_sum = std::move(sum);
            sum = add_numbers(old_sum, numbers[i]);
        }
        if (are_enabled_debug_messages)
        {
            outf("= ");
            print_root_left_right(sum);
        }
    }
    
    outf("{}", magnitude(sum));
}

void solve_2(const Input& input)
{
    outf("\nPart {}: ", 2);

    std::vector<Number> numbers;
    numbers.reserve(input.size());
    for (auto& e : input)
    {
        size_t index = 0;
        numbers.push_back(parse_number(e, index));
    }
    int64_t max_magnitude = 0;
    for (size_t i = 0; i < input.size(); i++)
    {
        for (size_t j = 0; j < input.size(); j++)
        {
            if (i != j)
            {
                auto sum1 = add_numbers(numbers[i], numbers[j]); // ij
                auto sum2 = add_numbers(numbers[j], numbers[i]); // ji
                
                auto mag = magnitude(sum1);
                if (mag > max_magnitude)
                {
                    max_magnitude = mag;
                }
                
                mag = magnitude(sum2);
                if (mag > max_magnitude)
                {
                    max_magnitude = mag;
                }
            }
        }
    }
    outf("{}", max_magnitude);
}

int main()
{
    auto input = read_input();
    solve_1(input);
    solve_2(input);

    return 0;
}
