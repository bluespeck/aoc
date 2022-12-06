#include "../outf.h"

#include<string>

using input_t = std::string;

input_t read()
{
    input_t input;
    
    std::getline(std::cin, input);

    return input;
}


void solve_1(const input_t& input)
{
    std::string accumulator;
    for (int i = 0; i < input.size(); i++)
    {
        if (size_t pos = accumulator.find(input[i]); pos != std::string::npos)
        {
            accumulator = accumulator.substr(pos + 1);
        }

        accumulator.push_back(input[i]);
        if (accumulator.size() == 4)
        {
            outf("{}\n", i + 1);
            break;
        }
    }

}

void solve_2(const input_t& input)
{
    std::string accumulator;
    for (int i = 0; i < input.size(); i++)
    {
        if (size_t pos = accumulator.find(input[i]); pos != std::string::npos)
        {
            accumulator = accumulator.substr(pos + 1);
        }

        accumulator.push_back(input[i]);
        if (accumulator.size() == 14)
        {
            outf("{}\n", i + 1);
            break;
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