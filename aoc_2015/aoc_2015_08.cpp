#include <ranges>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <unordered_map>

using input_t = std::vector<std::string>;

input_t
read_input()
{
    input_t input;

    std::string line;
    while (std::getline(std::cin, line))
    {
        input.push_back(line);
    };

    return input;
}

void
solve_1(const input_t& input)
{  
    std::vector<std::string> in_mem_strings;
    for (auto& e : input)
    {
        if (e.size() == 2)
        {
            in_mem_strings.push_back("");
        }
        else
        {
            std::string in_mem_string;
            for (size_t i = 1; i < e.size() - 1;)
            {
                if (e[i] == '\\')
                {
                    if (e[i + 1] == 'x')
                    {
                        in_mem_string += static_cast<char>(stoi(e.substr(i + 2, 2), 0, 16));
                        i += 4;
                    }
                    else
                    {
                        in_mem_string += e[i + 1];
                        i += 2;
                    }
                }
                else
                {
                    in_mem_string += e[i];
                    i++;
                }
            }
            in_mem_strings.push_back(in_mem_string);
        }
    }

    size_t code_size = 0;
    for (auto& e : input)
    {
        code_size += e.size();
    }
    size_t in_mem_size = 0;
    for (auto& e : in_mem_strings)
    {
        in_mem_size += e.size();
    }

    std::cout << code_size - in_mem_size << "\n";
}

void
solve_2(const input_t& input)
{
    std::vector<std::string> encoded_strings;
    for (auto& e : input)
    {
        if (e.size() == 2)
        {
            encoded_strings.push_back(R"("\"\"")");
        }
        else
        {
            std::string encoded_string = R"("\")";
            for (size_t i = 1; i < e.size() - 1; i++)
            {
                if (e[i] == '\\')
                {
                    encoded_string += R"(\\)";
                }
                else if(e[i] == '"')
                {
                    encoded_string += R"(\")";
                }
                else
                {
                    encoded_string += e[i];
                }
            }
            encoded_string += R"(\"")";

            encoded_strings.push_back(encoded_string);
        }
    }

    size_t code_size = 0;
    for (auto& e : input)
    {
        code_size += e.size();
    }
    size_t encoded_size = 0;
    for (auto& e : encoded_strings)
    {
        encoded_size += e.size();
    }

    std::cout << encoded_size - code_size << "\n";
}

int
main()
{
    input_t input = read_input();

    solve_1(input);
    solve_2(input);

    return 0;
}