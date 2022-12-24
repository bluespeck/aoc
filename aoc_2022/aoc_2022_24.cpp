#include "../outf.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <string>
#include <vector>

// had to reserve more stack size for this to work (same as day 18)

enum direction_t: char
{
    right,
    down,
    left,
    up
};

constexpr std::array<char, 4> facings = { '>', 'v', '<', '^' };

constexpr std::array<int32_t, 4> di = { 0, 1, 0, -1 };
constexpr std::array<int32_t, 4> dj = { 1, 0, -1, 0 };

struct position_t
{
    int64_t i, j;
};

struct blizzard_t
{
    position_t pos;
    direction_t dir;
};

using blizzard_vec_t = std::vector<blizzard_t>;
using blizzard_timeline_t = std::vector<blizzard_vec_t>;

struct input_t
{
    size_t grid_width;
    size_t grid_height;
    blizzard_vec_t blizzards;
};

input_t read()
{
    std::vector<std::string> grid;
    std::string line;
    while (std::getline(std::cin, line))
    {
        grid.push_back(line);
    }

    input_t input;

    input.grid_height = grid.size();
    input.grid_width = grid[0].size();

    for (int64_t i = 0; i < input.grid_height; i++)
    {
        for (int64_t j = 0; j < input.grid_width; j++)
        {
            for (char k = right; k <= up; k++)
            {
                if (grid[i][j] == facings[k])
                {
                    input.blizzards.emplace_back(position_t{ i, j }, direction_t{ k });
                }
            }
        }
    }

    return input;
}

void advance_blizzards(std::vector<blizzard_t>& blizzards, size_t grid_height, size_t grid_width)
{
    for (auto& e : blizzards)
    {
        e.pos.i = (grid_height - 2 + e.pos.i - 1 + di[e.dir]) % (grid_height - 2) + 1;
        e.pos.j = (grid_width - 2 + e.pos.j - 1 + dj[e.dir]) % (grid_width - 2) + 1;
    }
}

void reverse_blizzards(std::vector<blizzard_t>& blizzards, size_t grid_height, size_t grid_width)
{
    for (auto& e : blizzards)
    {
        e.pos.i = (grid_height - 2 + e.pos.i - 1 - di[e.dir]) % (grid_height - 2) + 1;
        e.pos.j = (grid_width - 2 + e.pos.j - 1 - dj[e.dir]) % (grid_width - 2) + 1;
    }
}

using grid_t = std::vector<std::vector<int64_t>>;
using grid_vec_t = std::vector<grid_t>;

void init_grid(grid_t& grid, const input_t& input, const size_t& max_distance)
{
    grid.resize(input.grid_height);
    for (auto& e : grid)
    {
        e.resize(input.grid_width, max_distance - 1);
        e[0] = max_distance;
        e[input.grid_width - 1] = max_distance;
    }
    for (auto& e : grid[0])
    {
        e = max_distance;
    }
    for (auto& e : grid[input.grid_height - 1])
    {
        e = max_distance;
    }
    grid[0][1] = max_distance - 1;
    grid[input.grid_height - 1][input.grid_width - 2] = max_distance - 1;
}

bool is_blizzard(const blizzard_vec_t& blizzards, const position_t& pos)
{
    return (std::find_if(blizzards.cbegin(), blizzards.cend(), [&pos](const blizzard_t& blizzard) {
        return blizzard.pos.i == pos.i && blizzard.pos.j == pos.j;
        }) != blizzards.cend());
}

void print_grid(const grid_t& grid, const blizzard_vec_t& blizzards, const position_t& elf_pos)
{
    std::vector<std::string> str_grid;
    str_grid.resize(grid.size() - 2);
    for (auto& e : str_grid)
    {
        e.resize(grid[0].size() - 2, '.');
    }
    add_padding(str_grid, '#', 1);
    str_grid[0][1] = '.';
    str_grid[str_grid.size() - 1][str_grid[0].size() - 2] = '.';
    str_grid[elf_pos.i][elf_pos.j] = 'E';
    for (auto& e : blizzards)
    {
        if (str_grid[e.pos.i][e.pos.j] != '.')
        {
            if (str_grid[e.pos.i][e.pos.j] >= '0' && str_grid[e.pos.i][e.pos.j] < '4')
            {
                str_grid[e.pos.i][e.pos.j]++;
            }
            else
            {
                str_grid[e.pos.i][e.pos.j] = '2';
            }
        }
        else
        {
            str_grid[e.pos.i][e.pos.j] = facings[e.dir];
        }
    }
    for (const auto& e : str_grid)
    {
        outf("{}\n", e);
    }
    outf("\n");
}

position_t destination;

void recurse(grid_vec_t& grids, position_t pos, const blizzard_timeline_t& blizzard_timeline, size_t minutes_passed)
{
    static const size_t grid_height = grids[0].size();
    static const size_t grid_width = grids[0][0].size();
    static const size_t max_distance = grid_height * grid_width;
    
    const auto& future_blizzards = blizzard_timeline[minutes_passed + 1];
    auto& grid = grids[minutes_passed];
    auto& ggrid = grids[minutes_passed + 1];
    if (pos.i == destination.i && pos.j == destination.j)
    {
        return;
    }
    //print_grid(grids[minutes_passed], blizzards, pos);
    for (char k = 0; k < 4; k++)
    {
        int64_t ii = pos.i + di[k];
        int64_t jj = pos.j + dj[k];
        //advance_blizzards(blizzards, grid_height, grid_width);
        
        if (ii >= 0 && ii < grid_height && jj >= 0 && jj <= grid_width && !is_blizzard(future_blizzards, { ii, jj })
            && grid[ii][jj] != max_distance && ggrid[ii][jj] > grid[pos.i][pos.j] + 1)
        {
            //outf("Minute {}, ", minutes_passed + 1);
            //outf("move {}:\n", facings[k]);
            ggrid[ii][jj] = grid[pos.i][pos.j] + 1;
            recurse(grids, { ii, jj }, blizzard_timeline, minutes_passed + 1);
        }
        //reverse_blizzards(blizzards, grid_height, grid_width);
    }

    // wait
    if(pos.i != destination.i || pos.j != destination.j)
    {
        //advance_blizzards(blizzards, grid_height, grid_width);
        if (!is_blizzard(future_blizzards, pos) && ggrid[pos.i][pos.j] > grid[pos.i][pos.j] + 1)
        {
            //outf("Minute {}, ", minutes_passed + 1);
            //outf("wait:\n");
            ggrid[pos.i][pos.j] = grid[pos.i][pos.j] + 1;
            recurse(grids, pos, blizzard_timeline, minutes_passed + 1);
        }
        //reverse_blizzards(blizzards, grid_height, grid_width);
    }
}

size_t solve(const input_t& input, const position_t& initial_pos, size_t initial_time)
{
    auto max_distance = input.grid_width * input.grid_height;
    auto blizzards = input.blizzards;

    grid_vec_t grids;
    blizzard_timeline_t blizzard_timeline;

    grids.resize(max_distance * 10);
    blizzard_timeline.resize(max_distance * 10);
    for (auto& e : grids)
    {
        init_grid(e, input, max_distance);
    }

    for (size_t i = 0; i < initial_time; i++)
    {
        advance_blizzards(blizzards, input.grid_height, input.grid_width);
    }
    for (size_t i = 0; i < blizzard_timeline.size(); i++)
    {
        blizzard_timeline[i] = blizzards;
        advance_blizzards(blizzards, input.grid_height, input.grid_width);
    }

    position_t elf_pos = initial_pos;
    grids[0][elf_pos.i][elf_pos.j] = 0;
    recurse(grids, elf_pos, blizzard_timeline, 0);

    auto min_time = max_distance;
    for (auto e : grids)
    {
        if (e[destination.i][destination.j] < min_time)
            min_time = e[destination.i][destination.j];
    }
    return min_time;
}

void solve_1(const input_t& input)
{
    destination = position_t{ static_cast<int64_t>(input.grid_height) - 1, static_cast<int64_t>(input.grid_width) - 2 };
    outf("{}\n", solve(input, { 0, 1 }, 0));
}

void solve_2(const input_t& input)
{
    position_t p1 = { 0, 1 };
    position_t p2 = position_t{ static_cast<int64_t>(input.grid_height) - 1, static_cast<int64_t>(input.grid_width) - 2 };
    
    destination = p2;
    auto t1 = solve(input, p1, 0);
    outf("t1={}\n", t1);
    destination = p1;
    auto t2 = solve(input, p2, t1);
    outf("t2={}\n", t2);
    destination = p2;
    auto t3 = solve(input, p1, t1 + t2);
    outf("t3={}\n", t3);
    outf("{}\n", t1 + t2 + t3);
}

int main()
{
    input_t input = read();
    solve_1(input);
    solve_2(input);

    return 0;
}