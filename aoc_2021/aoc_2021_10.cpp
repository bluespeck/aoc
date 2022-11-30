#include <array>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "outf.h"

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

int64_t compute_fail_score(const std::string& line, std::vector<char>& chunk_stack)
{
    int64_t score = 0;
    
    for (auto ch : line)
    {
        if (ch == '(' || ch == '[' || ch == '{' || ch == '<')
        {
            chunk_stack.push_back(ch);
        }
        else
        {
            if (ch == ')' && '(' != chunk_stack.back())
            {
                score = 3;
                break;
            }
            else if (ch == ']' && '[' != chunk_stack.back())
            {
                score = 57;
                break;
            }
            else if (ch == '}' && '{' != chunk_stack.back())
            {
                score = 1197;
                break;
            }
            else if (ch == '>' && '<' != chunk_stack.back())
            {
                score = 25137;
                break;
            }
            else
                chunk_stack.pop_back();
        }
    }
    return score;
}

void solve_1(const Input& input)
{
    outf("\nPart {}: ", 1);
    int64_t sum = 0;
    for (auto &line : input)
    {
        std::vector<char> chunk_stack;
        sum += compute_fail_score(line, chunk_stack);
    }
    outf("{}", sum);
}

void solve_2(const Input& input)
{
    outf("\nPart {}: ", 2);
    std::vector<int64_t> scores;
    for (auto& line : input)
    {
        std::vector<char> chunk_stack;
        auto score = compute_fail_score(line, chunk_stack);
        
        if (score == 0 && chunk_stack.size() > 0)
        {
            for (int64_t j = chunk_stack.size() - 1; j >= 0; j--)
            {
                auto ch = chunk_stack[j];
                score = score * 5 + (ch == '(' ? 1 : ch == '[' ? 2 : ch == '{' ? 3 : ch == '<' ? 4 : 0);
            }
            scores.push_back(score);
        }
    }
    std::sort(scores.begin(), scores.end());
    outf("{}", scores[scores.size() / 2]);
}

int main()
{
    auto input = read_input();
    solve_1(input); 
    solve_2(input);

    return 0;
}
