#include <cassert>
#include <iostream>
#include <vector>
#include "outf.h"

struct Point
{
    int32_t x, y;
};

struct Input
{
    std::vector<std::pair<Point, Point>> edges;
    int32_t max_x, max_y;
};

Input read_input()
{
    Input input;
    input.max_x = 0;
    input.max_y = 0;

    while (std::cin)
    {
        std::string line;
        std::getline(std::cin, line);
        if (line.size())
        {
            auto point_strs = split_string(line, " -> ");
            assert(point_strs.size() == 2);
            auto coord_strs_0 = split_string(point_strs[0], ",");
            auto coord_strs_1 = split_string(point_strs[1], ",");
            assert(coord_strs_0.size() == 2);
            assert(coord_strs_1.size() == 2);
            Point p1{ std::stol(coord_strs_0[0]), std::stol(coord_strs_0[1]) };
            Point p2{ std::stol(coord_strs_1[0]), std::stol(coord_strs_1[1]) };
            if (p1.x > input.max_x)
                input.max_x = p1.x;
            if (p2.x > input.max_x)
                input.max_x = p2.x;
            if (p1.y > input.max_y)
                input.max_y = p1.y;
            if (p2.y > input.max_y)
                input.max_y = p2.y;
            input.edges.emplace_back(p1, p2);
        }
    }
    return input;
}


using Grid = std::vector<std::vector<uint32_t>>;

Grid create_initialized_grid(const Input& input)
{
    Grid grid;

    grid.resize(input.max_y + 1);
    for (auto& row : grid)
        row.resize(input.max_x + 1, 0);

    return grid;
}

uint32_t count_intersections(const Grid& grid)
{
    uint32_t count = 0;
    for (auto& row : grid)
        for (auto e : row)
            if (e >= 2)
                count++;

    return count;
}

void solve_1(const Input& input)
{
    outf("\nPart {}: ", 1);

    auto grid = create_initialized_grid(input);

    for (auto& [p1, p2] : input.edges)
    {
        int32_t dx = p1.x < p2.x ? 1 : (p1.x > p2.x ? -1 : 0);
        int32_t dy = p1.y < p2.y ? 1 : (p1.y > p2.y ? -1 : 0);
        if (dx == 0 || dy == 0)
        {
            for (int32_t i = p1.x, j = p1.y; i != p2.x + dx || j != p2.y + dy; i += dx, j += dy)
            {
                grid[j][i]++;
            }
        }
    }
    
    outf("{}", count_intersections(grid));
}

void solve_2(const Input& input)
{
    outf("\nPart {}: ", 2);

    auto grid = create_initialized_grid(input);

    for (auto& [p1, p2] : input.edges)
    {
        int32_t dx = p1.x < p2.x ? 1 : (p1.x > p2.x ? -1 : 0);
        int32_t dy = p1.y < p2.y ? 1 : (p1.y > p2.y ? -1 : 0);
        for (int32_t i = p1.x, j = p1.y;
            i != p2.x + dx || j != p2.y + dy;
            i += dx, j += dy)
        {
            grid[j][i]++;
        }
    }

    outf("{}", count_intersections(grid));
}

int main()
{
    auto input = read_input();

    solve_1(input);
    solve_2(input);

    return 0;
}
