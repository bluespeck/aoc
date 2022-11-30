import std.core;

using operand_t = int64_t;
using operator_t = char;

struct expression_t
{
    
};

using input_t = std::vector<std:: string>;

input_t
read_input()
{
    input_t input;

    
    while (std::cin)
    {
        std::string line;
        if (std::getline(std::cin, line))
        {
            input.push_back(line);
        }
    }
    
    return input;
}

int64_t evaluate_1(const std::string& expr, size_t& exp_index)
{
    int64_t result = 0;
    char current_op = '+';
    operand_t val = 0;

    for (size_t i = exp_index; i < expr.size();)
    {
        auto ch = expr[i];
        if (ch == '(')
        {
            i++;
            val = evaluate_1(expr, i);
        }
        else if (ch == ')')
        {
            switch (current_op)
            {
            case '+': result += val; break;
            case '*': result *= val; break;
            }
            exp_index = i + 1;
            return result;
        }
        else if (isdigit(ch))
        {
            while (isdigit(ch))
            {
                val = val * 10 + ch - '0';
                ch = expr[++i];
            }
        }
        else {
            switch (current_op)
            {
            case '+': result += val; break;
            case '*': result *= val; break;
            }
            val = 0;
            current_op = ch;
            i++;
        }
    }
    switch (current_op)
    {
    case '+': result += val; break;
    case '*': result *= val; break;
    }
    exp_index = expr.size();
    return result;
}

void
solve_1(input_t& input)
{
    int64_t result = 0;
    for (auto& line : input)
    {
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        size_t i = 0;
        result += evaluate_1(line, i);
    }
    std::cout << result << std::endl;

}

int64_t evaluate_2(const std::string& expr, size_t& exp_index, int64_t initial_val = 0)
{
    int64_t result = initial_val;
    char current_op = '+';
    operand_t val = 0;
    size_t initial_index = exp_index;
    for (size_t i = exp_index; i < expr.size();)
    {
        auto ch = expr[i];
        if (ch == '(')
        {
            i++;
            val = evaluate_2(expr, i);
        }
        else if (ch == ')')
        {
            switch (current_op)
            {
            case '+': 
            {
                result += val; 
                if (initial_val)
                {
                    exp_index = i;
                    return result;
                }
                break;
            }
            case '*': result *= val; break;
            }
            exp_index = i + 1;
            return result;
        }
        else if (isdigit(ch))
        {
            while (isdigit(ch))
            {
                val = val * 10 + ch - '0';
                ch = expr[++i];
            }
        }
        else {
            switch (current_op)
            {
            case '+': 
                result += val; 
                if (ch == '*' && initial_val)
                {
                    exp_index = i;
                    return result;
                }
                break;
            case '*':
                if (ch == '+')
                {
                    val = evaluate_2(expr, i, val);
                    continue;
                }
                else
                {
                    result *= val;
                }
                break;
            }
            i++;
            val = 0;
            current_op = ch;
        }
    }
    switch (current_op)
    {
    case '+': result += val; break;
    case '*': result *= val; break;
    }
    exp_index = expr.size();
    return result;
}

void
solve_2(input_t& input)
{
    int64_t result = 0;
    for (auto& line : input)
    {
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        size_t i = 0;
        int64_t prev_result = result;
        result += evaluate_2(line, i);
        if (prev_result >= result)
        {
            return;
        }
    }
    std::cout << result << std::endl;
}

int
main()
{
    input_t input = read_input();
    solve_1(input);
    solve_2(input);

    return 0;
}