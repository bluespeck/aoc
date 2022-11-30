import std.core;

using slice_t = std::vector<std::string>;
using grid_t = std::vector<slice_t>;

using input_t = slice_t;

input_t
read_input()
{
    input_t input;
    
    while (std::cin)
    {
        std::string line;
        if (std::getline(std::cin, line))
        {
            input.push_back(line);
        }
    }
    
    return input;
}

void
solve_1(const input_t& input)
{
    grid_t grid;
    grid.push_back(input);
                                                            
    std::array<int64_t, 26> dx = { -1, -1, -1, -1, -1, -1, -1, -1, -1,  0,  0,  0,  0, 0,  0, 0, 0,  1,  1,  1,  1, 1, 1,  1, 1, 1 };
    std::array<int64_t, 26> dy = { -1, -1, -1,  0,  0,  0,  1,  1,  1, -1, -1, -1,  0, 0,  1, 1, 1, -1, -1, -1,  0, 0, 0,  1, 1, 1 };
    std::array<int64_t, 26> dz = { -1,  0,  1, -1,  0,  1, -1,  0,  1, -1,  0,  1, -1, 1, -1, 0, 1, -1,  0,  1, -1, 0, 1, -1, 0, 1 };

    int64_t max_steps = 6;

    std::string empty_row(2 * max_steps + input[0].size(), '.');
    slice_t empty_slice(2 * max_steps + input.size(), empty_row);
    std::string max_steps_empty_space(max_steps, '.');

    for (auto& row : grid[0])
    {
        row = max_steps_empty_space + row;
        row += max_steps_empty_space;
    }
    for (int i = 0; i < max_steps; i++)
    {
        grid[0].insert(grid[0].begin(), empty_row);
        grid[0].push_back(empty_row);
    }
    for (int i = 0; i < max_steps; i++)
    {
        grid.insert(grid.begin(), empty_slice);
        grid.push_back(empty_slice);
    }

    auto in_bounds = [](int64_t value, int64_t lower_bound, int64_t upper_bound) 
    {
        return (lower_bound <= value && value < upper_bound);
    };

    for (int step = 1; step <= max_steps; step++)
    {
        auto previous_grid = grid;
        for(int64_t z = 0; z < grid.size(); z++)
        {
            auto& slice = grid[z];
            for (int64_t y = 0; y < slice.size(); y++)
            {
                auto& row = slice[y];
                for (int64_t x = 0; x < row.size(); x++)
                {
                    auto& cube = row[x];
                    size_t active_neighbor_count = 0;
                    for (int8_t i = 0; i < 26; i++)
                    {
                        auto xx = x + dx[i];
                        auto yy = y + dy[i];
                        auto zz = z + dz[i];

                        if (in_bounds(zz, 0, previous_grid.size()) 
                            && in_bounds(yy, 0, previous_grid[zz].size()) 
                            && in_bounds(xx, 0, previous_grid[zz][yy].size()))
                        {
                            if (previous_grid[zz][yy][xx] == '#')
                            {
                                active_neighbor_count++;
                            }
                        }
                    }

                    if(previous_grid[z][y][x] == '#')
                    { 
                        if (active_neighbor_count != 2 && active_neighbor_count != 3)
                            cube = '.';
                    }
                    else
                    {
                        if (active_neighbor_count == 3)
                            cube = '#';
                    }
                }
            }
        }
        /*{
            size_t total_active_count = 0;
            std::cout << "\nAfter " << step << " cycles: \n";
            for (int64_t z = max_steps - step; z < max_steps + step + 1; z++)
            {
                std::cout << "slice " << z - max_steps << std::endl;
                auto& slice = grid[z];
                for (int64_t y = 0; y < slice.size(); y++)
                {
                    auto& row = slice[y];
                    std::cout << row << "\n";
                    total_active_count += std::ranges::count(row, '#');
                }
                std::cout << std::endl;
            }
            std::cout << "total: " << total_active_count << "\n";
        }*/

    }

    size_t total_active_count = 0;
    for (int64_t z = 0; z < grid.size(); z++)
    {
        auto& slice = grid[z];
        for (int64_t y = 0; y < slice.size(); y++)
        {
            auto& row = slice[y];
            total_active_count += std::ranges::count(row, '#');
        }
    }

    std::cout << total_active_count << std::endl;
}

void
solve_2(input_t& input)
{
    using grid4_t = std::vector<grid_t>;
    grid_t input_grid;
    input_grid.push_back(input);
    grid4_t grid;
    grid.push_back(input_grid);

    std::array<std::vector<int64_t>, 4> delta;
    for (int64_t ww = -1; ww <= 1; ww++)
    for (int64_t zz = -1; zz <= 1; zz++)
    for (int64_t yy = -1; yy <= 1; yy++)
    for (int64_t xx = -1; xx <= 1; xx++)
    {
        if (ww == 0 && zz == 0 && yy == 0 && xx == 0)
        {
            continue;
        }
        delta[0].push_back(ww);
        delta[1].push_back(zz);
        delta[2].push_back(yy);
        delta[3].push_back(xx);
    }

    int64_t max_steps = 6;

    std::string empty_row(2 * max_steps + input[0].size(), '.');
    slice_t empty_slice(2 * max_steps + input.size(), empty_row);
    grid_t empty_cube(2 * max_steps + 1, empty_slice);

    std::string max_steps_empty_space(max_steps, '.');

    for (auto& row : grid[0][0])
    {
        row = max_steps_empty_space + row;
        row += max_steps_empty_space;
    }
    for (int i = 0; i < max_steps; i++)
    {
        grid[0][0].insert(grid[0][0].begin(), empty_row);
        grid[0][0].push_back(empty_row);
    }
    for (int i = 0; i < max_steps; i++)
    {
        grid[0].insert(grid[0].begin(), empty_slice);
        grid[0].push_back(empty_slice);
    }
    for (int i = 0; i < max_steps; i++)
    {
        grid.insert(grid.begin(), empty_cube);
        grid.push_back(empty_cube);
    }


    auto in_bounds = [](int64_t value, int64_t lower_bound, int64_t upper_bound)
    {
        return (lower_bound <= value && value < upper_bound);
    };

    for (int step = 1; step <= max_steps; step++)
    {
        auto previous_grid = grid;
        for (int64_t w = 0; w < grid.size(); w++)
        {
            auto& cube = grid[w];
            for (int64_t z = 0; z < cube.size(); z++)
            {
                auto& slice = cube[z];
                for (int64_t y = 0; y < slice.size(); y++)
                {
                    auto& row = slice[y];
                    for (int64_t x = 0; x < row.size(); x++)
                    {
                        auto& cell = row[x];
                        size_t active_neighbor_count = 0;
                        for (int8_t i = 0; i < 80; i++)
                        {
                            auto xx = x + delta[3][i];
                            auto yy = y + delta[2][i];
                            auto zz = z + delta[1][i];
                            auto ww = w + delta[0][i];

                            if (in_bounds(ww, 0, previous_grid.size())
                                && in_bounds(zz, 0, previous_grid[ww].size())
                                && in_bounds(yy, 0, previous_grid[ww][zz].size())
                                && in_bounds(xx, 0, previous_grid[ww][zz][yy].size()))
                            {
                                if (previous_grid[ww][zz][yy][xx] == '#')
                                {
                                    active_neighbor_count++;
                                }
                            }
                        }

                        if (previous_grid[w][z][y][x] == '#')
                        {
                            if (active_neighbor_count != 2 && active_neighbor_count != 3)
                                cell = '.';
                        }
                        else
                        {
                            if (active_neighbor_count == 3)
                                cell = '#';
                        }
                    }
                }
            }
        }

        /*{
            size_t total_active_count = 0;
            std::cout << "\nAfter " << step << " cycles: \n";
            for (int64_t w = max_steps - step; w < max_steps + step + 1; w++)
            {
                for (int64_t z = max_steps - step; z < max_steps + step + 1; z++)
                {
                    std::cout << "z= " << z - max_steps << "w = " << w - max_steps << std::endl;
                    auto& slice = grid[w][z];
                    for (int64_t y = 0; y < slice.size(); y++)
                    {
                        auto& row = slice[y];
                        std::cout << row << "\n";
                        total_active_count += std::ranges::count(row, '#');
                    }
                }
                std::cout << std::endl;
            }
            std::cout << "total: " << total_active_count << "\n";
        }*/

    }

    size_t total_active_count = 0;
    for (int64_t w = 0; w < grid.size(); w++)
    {
        auto& cube = grid[w];
        for (int64_t z = 0; z < cube.size(); z++)
        {
            auto& slice = cube[z];
            for (int64_t y = 0; y < slice.size(); y++)
            {
                auto& row = slice[y];
                total_active_count += std::ranges::count(row, '#');
            }
        }
    }

    std::cout << total_active_count << std::endl;
}

int
main()
{
    input_t input = read_input();

    solve_1(input);
    solve_2(input);

    return 0;
}