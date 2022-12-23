#include "../outf.h"

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

using grid_t = std::vector<std::string>;
using input_t = grid_t;
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

enum dir_t : char
{
    north,
    north_east,
    east,
    south_east,
    south,
    south_west,
    west,
    north_west,
    no_dir
};

using directions_t = std::vector<dir_t>;

directions_t all_dirs = { north, north_east, east, south_east, south, south_west, west, north_west };

bool is_elf(char c)
{
    return c == '#' || c >= north && c <= no_dir;
}

char& get_cell_to_the(dir_t rel_pos, grid_t& grid, size_t i, size_t j)
{
    switch (rel_pos)
    {
    case north:
        return grid[i - 1][j];
    case north_east:
        return grid[i - 1][j + 1];
    case east:
        return grid[i][j + 1];
    case south_east:
        return grid[i + 1][j + 1];
    case south:
        return grid[i + 1][j];
    case south_west:
        return grid[i + 1][j - 1];
    case west:
        return grid[i][j - 1];
    case north_west:
        return grid[i - 1][j - 1];
    case no_dir:
        return grid[i][j];
    default:
        assert(false);
    }
    return grid[i][j];
}

size_t count_neigboring_elves(grid_t& grid, size_t i, size_t j)
{
    size_t count = 0;
    for (auto dir : all_dirs)
    {
        char cell = get_cell_to_the(dir, grid, i, j);
        if (is_elf(cell))
        {
            count++;
        }
    }
    return count;
}

size_t count_elves_moving_in(grid_t& grid, size_t i, size_t j, dir_t& move_from_dir)
{
    size_t count = 0;
    for (auto dir : all_dirs)
    {
        char cell = get_cell_to_the(dir, grid, i, j);
        if (is_elf(cell) && cell != no_dir && cell == (dir + 4) % 8)
        {
            move_from_dir = dir;
            count++;
        }
    }
    return count;
}

void print_grid(grid_t& grid)
{
    for (size_t i = 0; i < grid.size(); i++)
    {
        for (size_t j = 0; j < grid[i].size(); j++)
        {
            outf("{}", is_elf(grid[i][j]) ? '#' : '.');
        }
        outf("\n");
    }
    outf("\n"); 
}

input_t make_trimmed_grid(const input_t& grid)
{ // trim grid
    size_t min_i = grid.size();
    size_t max_i = 0;
    size_t min_j = grid[0].size();
    size_t max_j = 0;

    for (size_t i = 0; i < grid.size(); i++)
    {
        for (size_t j = 0; j < grid[i].size(); j++)
        {
            if (is_elf(grid[i][j]))
            {
                if (min_i > i) min_i = i;
                if (max_i < i) max_i = i;
                if (min_j > j) min_j = j;
                if (max_j < j) max_j = j;
            }
        }
    }

    input_t new_grid;
    for (size_t i = min_i; i <= max_i; i++)
    {
        new_grid.push_back(grid[i].substr(min_j, max_j - min_j + 1));
    }
    return new_grid;
}

void choose_direction_for_elf(grid_t& grid, size_t i, size_t j, const directions_t& dirs_to_consider)
{
    for (auto dir : dirs_to_consider)
    {
        if (dir == north 
            && !is_elf(get_cell_to_the(north, grid, i, j)) 
            && !is_elf(get_cell_to_the(north_east, grid, i, j))
            && !is_elf(get_cell_to_the(north_west, grid, i, j)))
        {
            grid[i][j] = north; // north
            break;
        }
        else if (dir == south
            && !is_elf(get_cell_to_the(south, grid, i, j))
            && !is_elf(get_cell_to_the(south_east, grid, i, j))
            && !is_elf(get_cell_to_the(south_west, grid, i, j)))
        {
            grid[i][j] = south; // south
            break;
        }
        else if (dir == west
            && !is_elf(get_cell_to_the(west, grid, i, j))
            && !is_elf(get_cell_to_the(north_west, grid, i, j))
            && !is_elf(get_cell_to_the(south_west, grid, i, j)))
        {
            grid[i][j] = west; // west
            break;
        }
        else if (dir == east
            && !is_elf(get_cell_to_the(east, grid, i, j))
            && !is_elf(get_cell_to_the(north_east, grid, i, j))
            && !is_elf(get_cell_to_the(south_east, grid, i, j)))
        {
            grid[i][j] = east; // east
            break;
        }
    }
}

void solve_1_2(const input_t& input)
{
    grid_t grid = input;

    //outf("Initial state.\n");
    //print_grid(grid);

    directions_t dirs_to_consider = { north, south, west, east };

    for (size_t round = 1; ; round++)
    {
        add_padding(grid, '.', 2);
        for (size_t i = 1; i < grid.size() - 1; i++)
        {
            for (size_t j = 1; j < grid[i].size() - 1; j++)
            {
                if (is_elf(grid[i][j]))
                {
                    if (count_neigboring_elves(grid, i, j) == 0)
                    {
                        grid[i][j] = no_dir;
                    }
                    else
                    {
                        choose_direction_for_elf(grid, i, j, dirs_to_consider);
                    }
                }
            }
        }
        auto grid_moving_phase = grid;
        bool elves_moved = false;
        for (size_t i = 1; i < grid.size() - 1; i++)
        {
            for (size_t j = 1; j < grid[i].size() - 1; j++)
            {
                if (!is_elf(grid[i][j]))
                {
                    dir_t move_from_dir;
                    auto elves_moving_in_count = count_elves_moving_in(grid, i, j, move_from_dir);
                    if (elves_moving_in_count == 1)
                    {
                        elves_moved = true;
                        auto& cell = get_cell_to_the(move_from_dir, grid_moving_phase, i, j);
                        grid_moving_phase[i][j] = cell;
                        cell = '.';
                    }
                }
            }
        }
        grid = grid_moving_phase;

        if (!elves_moved)
        {
            outf("{} is the first round where the elves didn't move.\n", round);
            return;
        }

        grid = make_trimmed_grid(grid);

        for (size_t i = 1; i < grid.size() - 1; i++)
        {
            for (size_t j = 1; j < grid[i].size() - 1; j++)
            {
                if (is_elf(grid[i][j]))
                {
                    grid[i][j] = '#';
                }
            }
        }

        if (round == 10)
        {
            size_t count = 0;
            for (size_t i = 0; i < grid.size(); i++)
            {
                for (size_t j = 0; j < grid[i].size(); j++)
                {
                    if (!is_elf(grid[i][j]))
                    {
                        count++;
                    }
                    else grid[i][j] = '#';
                }
            }
            outf("{} ground tiles in the elf-occupied rectangle.\n", count);
        }

        //outf("End of round {}.\n", round);
        //print_grid(grid);

        std::rotate(dirs_to_consider.begin(), dirs_to_consider.begin() + 1, dirs_to_consider.end());
    }
}

int main()
{
    input_t input = read();
    solve_1_2(input);

    return 0;
}