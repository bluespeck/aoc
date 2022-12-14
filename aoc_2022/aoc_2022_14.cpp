#include "../outf.h"

#include <string>
#include <vector>

struct point_t
{
    int64_t i, j;
};

using path_t = std::vector<point_t>;

struct input_t
{
    std::vector<path_t> paths;
    int64_t min_i = 99990;
    int64_t min_j = 99990;
    int64_t max_i = 0;
    int64_t max_j = 0;
};

input_t read()
{
    input_t input;
    std::string line;
    
    while (std::getline(std::cin, line))
    {
        path_t path;
        auto parts = split_string(line, " -> ");
        for (auto& e : parts)
        {
            auto coords = split_string(e, ",");
            int64_t ci = std::stoll(coords[1]);
            int64_t cj = std::stoll(coords[0]);
            if (ci > input.max_i)
            {
                input.max_i = ci;
            }
            if (cj > input.max_j)
            {
                input.max_j = cj;
            }
            if (ci < input.min_i)
            {
                input.min_i = ci;
            }
            if (cj < input.min_j)
            {
                input.min_j = cj;
            }
            path.emplace_back(ci, cj);
        }
        input.paths.push_back(path);
    }
    return input;
}

using grid_t = std::vector<std::vector<char>>;

void print_grid(const input_t& input, grid_t& grid)
{
    for (int i = 0; i <= input.max_i; i++)
    {
        for (int j = input.min_j; j <= input.max_j; j++)
        {
            outf("{}",grid[i][j]);
        };
        outf("\n");
    }
    std::cout << '\n';
}


grid_t make_grid(const input_t& input, bool is_second_prob = false)
{
    grid_t grid;
    grid.resize(input.max_i + 2);
    for (size_t i = 0; i < grid.size(); i++)
    {
        grid[i].resize(input.max_j + +(is_second_prob ? input.max_i + 1 : 1), '.');
    }

    for (auto& e: input.paths)
    {
        for (size_t i = 0; i < e.size() - 1; i++)
        {
            if (e[i].i == e[i + 1].i)
            {
                // horizontal line
                if (e[i].j < e[i + 1].j)
                {
                    for (size_t j = e[i].j; j <= e[i + 1].j; j++)
                    {
                        grid[e[i].i][j] = '#';
                    }
                }
                else
                {
                    for (size_t j = e[i+1].j; j <= e[i].j; j++)
                    {
                        grid[e[i].i][j] = '#';
                    }
                }
            }
            else
            {
                // vertical line
                if (e[i].i < e[i + 1].i)
                {
                    for (size_t j = e[i].i; j <= e[i + 1].i; j++)
                    {
                        grid[j][e[i].j] = '#';
                    }
                }
                else
                {
                    for (size_t j = e[i + 1].i; j <= e[i].i; j++)
                    {
                        grid[j][e[i].j] = '#';
                    }
                }
            }
        }
    }

    return grid;
}

void simulate_sand(const input_t&input, grid_t& grid, const point_t& sand_source, bool is_second_prob = false)
{
    while (true)
    {
        point_t current_grain_pos = sand_source;
        
        for(size_t i = current_grain_pos.i; i < grid.size() - 1; i++)
        { 
            auto& current_grid_cell = grid[i][current_grain_pos.j];
            if (current_grid_cell == '.')
            {
                current_grain_pos.i = i;
                continue;
            }
            else if (current_grid_cell == '#' || current_grid_cell == 'o')
            {
                if (grid[i][current_grain_pos.j - 1] == '.')
                {
                    current_grain_pos.i = i;
                    current_grain_pos.j--;
                    continue;
                }
                else if (grid[i][current_grain_pos.j + 1] == '.')
                {
                    current_grain_pos.i = i;
                    current_grain_pos.j++;
                    continue;
                }
                else
                {
                    grid[current_grain_pos.i][current_grain_pos.j] = 'o';
                    break;
                }
            }
        }
        if (is_second_prob)
        {
            if (grid[sand_source.i][sand_source.j] == 'o')
            {
                break;
            }
        }
        else if (current_grain_pos.i >= input.max_i)
        {
            break;
        }
        
    }
}

int64_t count_grains(const grid_t& grid)
{
    size_t count = 0;
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            if (grid[i][j] == 'o')
            {
                count++;
            }
        }
    }
    return count;
}

void solve_1(const input_t& input)
{
    grid_t grid = make_grid(input);
    point_t source = { 0, 500 };
    simulate_sand(input, grid, source);
    print_grid(input, grid);
    outf("Grains on the structure before starting to fall forever {}\n", count_grains(grid));
}

void solve_2(const input_t& input)
{
    auto input_2 = input;

    point_t source = { 0, 500 };

    input_2.max_i += 2;
    input_2.max_j = source.j + input_2.max_i;
    input_2.min_j = source.j - input_2.max_i;

    grid_t grid = make_grid(input_2, true);
    for (size_t j = 0; j < grid[0].size(); j++)
    {
        grid[input_2.max_i][j] = '#';
    }

    simulate_sand(input_2, grid, source, true);
    print_grid(input_2, grid);
    outf("Grains on the structure before the source is blocked {}\n", count_grains(grid));
}

int main()
{
    input_t input = read();
    solve_1(input);
    solve_2(input);

    return 0;
}