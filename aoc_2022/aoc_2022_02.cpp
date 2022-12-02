#include "../outf.h"

#include <cassert>
#include <string>
#include <vector>

struct round_t
{
    char p1, p2;
};

using input_t = std::vector<round_t>;
input_t read()
{
    input_t input;
    std::string line;
    while (std::getline(std::cin, line))
    {
        assert(line.size() == 3);
        char p1, p2;
        p1 = line[0];
        p2 = line[2];
        input.emplace_back(p1, p2);
    }

    return input;
}


void solve_1(const input_t& input)
{
    // a rock
    // b paper
    // c scissors
    // x rock
    // y paper
    // z scissors
    size_t score = 0;
    for (auto e : input)
    {
        if ((e.p1 - 'A' + 1) % 3 + 'X' == e.p2)
        {
            score += 6;
        }
        else if (e.p1 - 'A' + 'X' == e.p2)
        {
            score += 3;
        }
        score += e.p2 - 'X' + 1;
    }
    outf("Score 1: {}\n", score);
}

void solve_2(const input_t& input)
{
    // a rock
    // b paper
    // c scissors
    // x lose
    // y tie
    // z win
    size_t score = 0;
    for (auto e : input)
    {
        if (e.p2 == 'Y')
        {
            score += 3;
            score += e.p1 - 'A' + 1;
        }
        else if (e.p2 == 'Z')
        {
            score += 6;
            score += (e.p1 - 'A' + 1) % 3 + 1;
        }
        else
        {
            score += (e.p1 - 'A' + 2) % 3 + 1;
        }
    }
    outf("Score 2: {}\n", score);
}

int main()
{
    input_t input = read();
    solve_1(input);
    solve_2(input);

    return 0;
}