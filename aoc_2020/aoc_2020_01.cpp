import std.core;

using input_t = std::vector<int64_t>;

input_t read_input()
{
    input_t input;

    while (std::cin)
    {
        int64_t value;
        if (std::cin >> value)
        {
            input.push_back(value);
        }
    }

    return input;
}

void solve_1(input_t& input)
{
    for(int i = 0; i < input.size(); i++)
        for(int j = i+1; j < input.size(); j++)
            if(input[i] + input[j] == 2020)
                std::cout << input[i] * input[j] << std::endl;
}      

void solve_2(input_t& input)
{
    for(int i = 0; i < input.size(); i++)
        for(int j = i+1; j < input.size(); j++)
            for (int k = j + 1; k < input.size(); k++)
            if(input[i] + input[j] + input[k]== 2020)
                std::cout << input[i] * input[j] * input[k] << std::endl;
}

int main()
{
    input_t input = read_input();

    solve_1(input);
    solve_2(input);

    return 0;
}