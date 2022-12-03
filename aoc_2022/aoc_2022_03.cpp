#include "../outf.h"


#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <string>
#include <vector>

using input_t = std::vector<std::string>;
input_t read()
{
    input_t input;
    std::string line;
    while (std::getline(std::cin, line))
    {
        input.push_back(line);
    }

    return input;
}

size_t to_priority(char ch)
{
    return std::islower(ch) ? ch - 'a' + 1 : ch - 'A' + 27;
}

void solve_1(const input_t& input)
{
    size_t sum = 0;

    for (auto& e : input)
    {
        assert(e.size() % 2 == 0);
        size_t half_size = e.size() / 2;

        sum += to_priority(*std::find_first_of(e.begin() + half_size, e.end(), e.begin(), e.begin() + half_size));
    }
    
    outf("{}\n", sum);
}

void solve_2(const input_t& input)
{
    size_t sum = 0;

    for (size_t i = 0; i < input.size(); i += 3)
    {
        for (char ch : input[i])
        {
            if (input[i + 1].find(ch) != std::string::npos
             && input[i + 2].find(ch) != std::string::npos)
            {
                sum += to_priority(ch);
                break;
            }
        }
    }

    outf("{}\n", sum);
}

int main()
{
    input_t input = read();
    solve_1(input);
    solve_2(input);

     return 0;
}