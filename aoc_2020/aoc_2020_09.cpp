import std.core;

using input_t = std::vector<int64_t>;


input_t
read_input()
{
    input_t input;
    while (std::cin)
    {
        int64_t number;
        std::cin >> number;
        input.push_back(number);
    }

    return input;
}

void
solve_1(input_t& input)
{
    for (int i = 25; i < input.size(); i++)
    {
        bool found = false;
        for (int j = i - 25; j < i - 1; j++)
            for (int k = j + 1; k < i; k++)
                if (input[j] + input[k] == input[i])
                {
                    found = true;
                    break;
                }
        if (!found)
        {
            std::cout << input[i] << std::endl;
            return;
        }
    }

}

void
solve_2(input_t& input)
{
    for (int i = 25; i < input.size(); i++)
    {
        bool found = false;
        for (int j = i - 25; j < i - 1 && !found; j++)
            for (int k = j + 1; k < i; k++)
                if (input[j] + input[k] == input[i])
                {
                    found = true;
                    break;
                }
        if (!found)
        {
            for (int j = 0; j < i - 1; j++)
            {
                int64_t minimum = input[j];
                int64_t maximum = input[j];
                int64_t sum = input[j];
                for (int k = j + 1; k < i; k++)
                {
                    minimum = minimum > input[k] ? input[k] : minimum;
                    maximum = maximum < input[k] ? input[k] : maximum;
                    sum += input[k];
                    if (input[i] == sum)
                    {
                        std::cout << (minimum + maximum) << std::endl;
                        return;
                    }
                    else if (input[i] < sum)
                        break;
                }
            }
            return;
        }
    }
}

int
main()
{
    input_t input = read_input();
    
    solve_1(input);
    solve_2(input);

    return 0;
}