#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include "outf.h"

struct Point
{
    int64_t x, y;
};
bool operator <(Point p1, Point p2) {
    return p1.x < p2.x || p1.x == p2.x && p1.y < p2.y;
}
bool operator ==(Point p1, Point p2) {
    return p1.x == p2.x && p1.y == p2.y ;
}

struct Fold
{
    char axis;
    int64_t coord;
};

struct Input
{
    std::vector<Point> points;
    std::vector<Fold> folds;
};

Input read_input()
{
    Input input;
    while (std::cin)
    {
        std::string line;
        std::getline(std::cin, line);
        if (line.size())
        {
            if (line[0] != 'f')
            {
                auto vals = split_string(line, ",");
                assert(vals.size() == 2);
                input.points.emplace_back(std::stoll(vals[0]), std::stoll(vals[1]));
            }
            else
            {
                auto vals = split_string(line, "=");
                input.folds.emplace_back(vals[0][vals[0].size() - 1], std::stoll(vals[1]));
            }
        }
    }
    return input;
}

void solve_1(const Input& input)
{
    outf("\nPart {}: ", 1);


}

void solve_1_2(const Input& _input)
{
    Input input = _input;
    size_t fold_count = 0;
    for (auto& fold: input.folds)
    {
        fold_count++;
        
        for (auto& [x, y] : input.points)
        {
            if (fold.axis == 'y' && y > fold.coord)
            {
                y -= 2 * (y - fold.coord);
            }
            else if (fold.axis == 'x' && x > fold.coord)
            {
                x -= 2 * (x - fold.coord);
            }

        }

        std::sort(input.points.begin(), input.points.end());
        input.points.erase(std::unique(input.points.begin(), input.points.end()), input.points.end());

        if (fold_count == 1)
        {
            outf("Part 1: {}\n", input.points.size());
        }
    }

    outf("Part 2:\n");
    for (int32_t i = 0; i < 6; i++)
    {
        for (int32_t j = 0; j < 40; j++)
        {
            bool found = false;
            for (auto [x, y] : input.points)
            {
                if (i == y && j == x)
                {
                    outf("#");
                    found = true;
                    break;
                }
            }
            if(!found)
            {
                outf(".");
            }
        }
        outf("\n");
    }
}

int main()
{
    auto input = read_input();
    solve_1_2(input);

    return 0;
}
